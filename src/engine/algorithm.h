#ifndef HEADER_INCLUDE__RECURSION__ALGORITHM__CHEBYSHEV_SEGMENTS__H
#define HEADER_INCLUDE__RECURSION__ALGORITHM__CHEBYSHEV_SEGMENTS__H
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


namespace b2draw {
namespace algorithm {

	
constexpr float HALF_PI = M_PI / 2;
constexpr float TWO_PI = 2 * M_PI;


/**
 * Given an angle and its cosine, calculate the sine.
 *
 * @param angle - the angle.
 * @param cosAngle - the cosine of the angle, `cos(angle)`.
 * @returns the value `sin(angle)`.
 */
static float
cosToSin(float const angle, float const cosAngle)
{
	bool const cond = std::remainder(angle, TWO_PI) < 0;
	return std::sqrt(1 - cosAngle * cosAngle) * (!cond - cond);
}


/**
 * Given an angle and its sine, calculate the cosine.
 *
 * @param angle - the angle.
 * @param sinAngle - the sine of the angle, `sin(angle)`.
 * @returns the value `cos(angle)`.
 */
static float
sinToCos(float const angle, float const sinAngle)
{
	bool const cond = std::remainder(angle + HALF_PI, TWO_PI) < 0;
	return std::sqrt(1 - sinAngle * sinAngle) * (!cond - cond);
}


/**
 * Calculate segmented outer vertices of a circle.
 *
 * Uses Chebyshev's algorithm to calculate sine and cosine of multiples of the
 * increment angle, then double angle formulae to get sine and cosine of these
 * multiples summed with the initial angle.
 *
 * See https://en.wikipedia.org/wiki/List_of_trigonometric_identities for the
 * algorithm and identities used.
 *
 * @tparam Container a container whose values can be constructed in-place by a
 * call to `emplace_back(float, float)`.
 *
 * @param vertices a container to be extended by the segment vertices.
 * @param centreX the x-coordinate of the circle's centre.
 * @param centreY the y-coordinate of the circle's centre.
 * @param radius the circle radius.
 * @param initialAngle the counter-clockwise angle from the Y-axis of the first
 * segment.
 *
 * Exception safety: if an `emplace_back` call throws, the container will have
 * been extended. Dependent code should reserve space in the container first,
 * or preferably use a temporary container and copy or swap the results, making
 * the algorithm safe.
 *
 * @code
 * std::vector<Vector2D> tmp;
 * vertices.reserve(numVertices);
 * chebyshevSegments(tmp, centreX, centreY, radius, angle, numVertices);
 * std::swap(tmp, m_vertices);
 * @endcode
 */
template <typename Vertex>
void
chebyshevSegments(
	Vertex* const pVertices,
	unsigned const numVertices,
	float const centreX,
	float const centreY,
	float const radius,
	float const initialAngle
)
{
	// Constants we will use for the double angle formulae.
	float const cosInitial = cosf(initialAngle);
	float const sinInitial = cosToSin(initialAngle, cosInitial);

	// cos((n - 1) * increment)
	float cosIncrN_1 = 1.0f;
	// sin((n - 1) * increment)
	float sinIncrN_1 = 0.0f;

	// The increment angle.
	float const increment = 2.0f * M_PI / float(numVertices);

	// cos(n * increment)
	float cosIncrN = cosf(increment);
	// sin(n * increment)
	float sinIncrN = cosToSin(increment, cosIncrN);

	// The constant multiplier in Chebyshev's algorithm, 2cos(increment).
	float const mult = 2 * cosIncrN;

	pVertices[0] = {
		centreX - radius * sinInitial,
		centreY + radius * cosInitial
	};
	pVertices[1] = {
		centreX - radius * (
			sinIncrN * cosInitial + sinInitial * cosIncrN
		),
		centreY + radius * (
			cosIncrN * cosInitial - sinIncrN * sinInitial
		)
	};

	for (unsigned i = 2; i < numVertices; ++i)
	{
		// Update the Chebyshev cos values.
		float chebyOld = cosIncrN;
		cosIncrN = mult * cosIncrN - cosIncrN_1;
		cosIncrN_1 = chebyOld;

		// Update the Chebyshev sin values.
		chebyOld = sinIncrN;
		sinIncrN = mult * sinIncrN - sinIncrN_1;
		sinIncrN_1 = chebyOld;

		// Use double angle formulae to get cos(n * increment + initialAngle)
		// and sin(n * increment + initialAngle).
		pVertices[i] = {
			centreX - radius * (
				sinIncrN * cosInitial + sinInitial * cosIncrN
			),
			centreY + radius * (
				cosIncrN * cosInitial - sinIncrN * sinInitial
			)
		};
	}
}


} // namespace algorithm
} // namespace b2draw
#endif // #ifndef HEADER_INCLUDE__RECURSION__ALGORITHM__CHEBYSHEV_SEGMENTS__H
