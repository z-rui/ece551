#include <cmath>
#include "circle.h"

void Circle::move(double dx, double dy)
{
	center.move(dx, dy);
}

inline double solveangle(double a, double b, double c)
{
	return std::acos((a*a + b*b - c*c) / (2.0*a*b));
}

inline double circsegarea(double r, double theta)
{
	return 0.5 * r*r * (theta - std::sin(theta));
}

double Circle::intersectionArea(const Circle &otherCircle)
{
	double d = center.distanceFrom(otherCircle.center);
	double r1 = this->radius;
	double r2 = otherCircle.radius;

	if (r1 + r2 <= d) {
		// no intersection
		return 0.0;
	}
	if (r1 > r2) {
		double temp = r1;
		r1 = r2;
		r2 = temp;
	}
	// now r1 <= r2
	if (d <= r2 - r1) {
		// one contains the other
		return M_PI * r1 * r1;
	}
	// two circles intersect
	double theta1, theta2;
	theta1 = 2.0 * solveangle(r1, d, r2);
	theta2 = 2.0 * solveangle(r2, d, r1);
	return circsegarea(r1, theta1) + circsegarea(r2, theta2);
}
