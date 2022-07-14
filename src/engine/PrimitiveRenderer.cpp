#include <cassert>
#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "algorithm.h"
#include "PrimitiveRenderer.h"
#include "gfx/GfxBase.h"

namespace b2draw {


PrimitiveRenderer::PrimitiveRenderer(
	GLint const positionAttribLocation,
	GLint const colourAttribLocation,
	unsigned const numCircleSegments
)
	:	m_vertices{}
	,	m_firstIndices{}
	,	m_polygonSizes{}
	,	m_tmpCircleBuffer{std::max(numCircleSegments, 3u)}
	,	m_vbo{0u}
	,	m_vao{0u}
{
	// This is a debugging library, so if we encounter GL errors, we prefer to
	// fail hard.
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		throw std::runtime_error{
			reinterpret_cast<char const*>(glGetString(error))};
	}

	glGenBuffers(1, &m_vbo);
	if (m_vbo == 0u) {
		throw std::runtime_error{"Invalid VBO"};
	}

	glGenVertexArrays(1, &m_vao);
	if (!m_vao)
	{
		glDeleteBuffers(1, &m_vbo);
		throw std::runtime_error{"Invalid VAO"};
	}

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	if (positionAttribLocation >= 0)
	{
		setPositionAttribLocation(positionAttribLocation);
	}
	error = glGetError();
	if (error != GL_NO_ERROR) {
		throw std::runtime_error{
			reinterpret_cast<char const*>(glGetString(error))};
	}

	if (colourAttribLocation >= 0)
	{
		setColourAttribLocation(colourAttribLocation);
	}
	error = glGetError();
	if (error != GL_NO_ERROR) {
		throw std::runtime_error{
			reinterpret_cast<char const*>(glGetString(error))};
	}
}


PrimitiveRenderer::PrimitiveRenderer(PrimitiveRenderer&& other) noexcept
	:	m_vertices{std::move(other.m_vertices)}
	,	m_firstIndices{std::move(other.m_firstIndices)}
	,	m_polygonSizes{std::move(other.m_polygonSizes)}
	,	m_tmpCircleBuffer{std::move(other.m_tmpCircleBuffer)}
	,	m_vbo{other.m_vbo}
	,	m_vao{other.m_vao}
{
	other.m_vbo = 0;
	other.m_vao = 0;
}


PrimitiveRenderer::~PrimitiveRenderer() noexcept
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}


void
PrimitiveRenderer::addPolygon(
	b2Vec2 const* const pVertices,
	int32 const numNewVertices,
	b2Color const& colour
)
{
	assert(numNewVertices != 0 && "Can't render an empty polygon!");
	// Reserve the space before we do anything.
	auto const totalVertices = m_vertices.size() + numNewVertices;
	m_vertices.reserve(totalVertices);
	auto const newPolygonCount = m_polygonSizes.size() + 1;
	m_firstIndices.reserve(newPolygonCount);
	m_polygonSizes.reserve(newPolygonCount);

	// Create a new polygon.
	m_firstIndices.push_back(m_vertices.size());
	m_polygonSizes.push_back(numNewVertices);

	// Copy vertices.
	b2Vec2 const* const pEnd = pVertices + numNewVertices;
	for (b2Vec2 const* pVertex = pVertices; pVertex < pEnd; ++pVertex)
	{
		m_vertices.emplace_back(*pVertex, colour);
	}
}


void
PrimitiveRenderer::addCircle(
	b2Vec2 const& centre,
	f32 const radius,
	b2Color const& colour,
	f32 const initialAngle
)
{
	algorithm::chebyshevSegments(
		m_tmpCircleBuffer.data(),
		m_tmpCircleBuffer.size(),
		centre.x,
		centre.y,
		radius,
		initialAngle
	);

	addPolygon(m_tmpCircleBuffer.data(), m_tmpCircleBuffer.size(), colour);
}


void
PrimitiveRenderer::addSegment(
	b2Vec2 const& begin,
	b2Vec2 const& end,
	b2Color const& colour
)
{
	m_vertices.reserve(m_vertices.size() + 2);
	auto const polygonCount = m_polygonSizes.size() + 1;
	m_polygonSizes.reserve(polygonCount);
	m_firstIndices.reserve(polygonCount);

	m_polygonSizes.push_back(2);
	m_firstIndices.push_back(m_vertices.size());
	m_vertices.emplace_back(begin, colour);
	m_vertices.emplace_back(end, colour);
}


void
PrimitiveRenderer::bufferData()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		m_vertices.size() * sizeof(Vertex),
		m_vertices.data(),
		GL_DYNAMIC_DRAW
	);
}


void
PrimitiveRenderer::render(GLenum const mode)
{
	if (empty()) {
		return;
	}
	glBindVertexArray(m_vao);
	glMultiDrawArrays(
		mode,
		m_firstIndices.data(),
		m_polygonSizes.data(),
		m_polygonSizes.size()
	);
}


void
PrimitiveRenderer::clear()
{
	m_vertices.clear();
	m_firstIndices.clear();
	m_polygonSizes.clear();
}


} // namespace b2draw
