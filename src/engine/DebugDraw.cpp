#include <cmath>

#include <box2d/box2d.h>

#include <gfx/GfxBase.h>
#include "algorithm.h"
#include "DebugDraw.h"


namespace b2draw {


DebugDraw::DebugDraw(
	GLint positionAttribLoc,
	GLint colourAttribLoc,
	unsigned numCircleSegments,
	f32 fillAlpha,
	f32 axisScale
)
	:	m_lineRenderer{positionAttribLoc, colourAttribLoc, numCircleSegments}
	,	m_fillRenderer{positionAttribLoc, colourAttribLoc, numCircleSegments}
	,	m_fillAlpha{fillAlpha}
	,	m_axisScale{axisScale}
{
}


DebugDraw::~DebugDraw() noexcept = default;


void
DebugDraw::DrawPolygon(
	b2Vec2 const* pVertices,
	int32 vertexCount,
	b2Color const& colour
)
{
	m_lineRenderer.addPolygon(pVertices, vertexCount, colour);
}

void
DebugDraw::DrawSolidPolygon(
	b2Vec2 const* pVertices,
	int32 vertexCount,
	b2Color const& colour
)
{
	b2Color fillColour{colour};
	fillColour.a = m_fillAlpha;

	m_fillRenderer.addPolygon(pVertices, vertexCount, fillColour);
}

void
DebugDraw::DrawCircle(
	b2Vec2 const& centre,
	f32 radius,
	b2Color const& colour
)
{
	m_lineRenderer.addCircle(centre, radius, colour);
}

void
DebugDraw::DrawSolidCircle(
	b2Vec2 const& centre,
	f32 radius,
	b2Vec2 const& axis,
	b2Color const& colour
)
{
	b2Color fillColour{colour};
	fillColour.a = m_fillAlpha;

	m_fillRenderer.addCircle(centre, radius, fillColour);
	m_lineRenderer.addSegment(
		centre,
		centre + radius * axis,
		b2Color{0.0f, 0.0f, 0.0f, 1.0f}
	);
}

void
DebugDraw::DrawSegment(
	b2Vec2 const& begin,
	b2Vec2 const& end,
	b2Color const& colour
)
{
	m_lineRenderer.addSegment(begin, end, colour);
}


void
DebugDraw::DrawPoint(
	b2Vec2 const& point,
	f32 size,
	b2Color const& colour
)
{
	constexpr int32 numVertices{4};
	b2Vec2 const vertices[numVertices] = {
		b2Vec2{point.x - 0.1f, point.y - 0.1f},
		b2Vec2{point.x + 0.1f, point.y - 0.1f},
		b2Vec2{point.x + 0.1f, point.y + 0.1f},
		b2Vec2{point.x - 0.1f, point.y + 0.1f}
	};
	DrawSolidPolygon(&vertices[0], numVertices, colour);
}


void
DebugDraw::DrawTransform(b2Transform const& xf)
{

	b2Vec2 end = xf.p + m_axisScale * xf.q.GetXAxis();
	m_lineRenderer.addSegment(xf.p, end, b2Color{1.0f, 0.0f, 0.0f});

	end = xf.p + m_axisScale * xf.q.GetYAxis();
	m_lineRenderer.addSegment(xf.p, end, b2Color{0.0f, 1.0f, 0.0f});
}


void
DebugDraw::BufferData()
{
	m_lineRenderer.bufferData();
	m_fillRenderer.bufferData();
}


void
DebugDraw::Render()
{
	m_lineRenderer.render(GL_LINE_LOOP);
	m_fillRenderer.render(GL_TRIANGLE_FAN);
}


void
DebugDraw::Clear()
{
	m_lineRenderer.clear();
	m_fillRenderer.clear();
}


} // namespace b2draw
