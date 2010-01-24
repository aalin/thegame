#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "vector3.hpp"

struct Triangle
{
	Triangle(Vector3 a, Vector3 b, Vector3 c) : a(a), b(b), c(c)
	{ }

	Triangle()
	{ }

	Vector3 getNormal()
	{
		Vector3 edge1 = a - b;
		Vector3 edge2 = b - c;
		return edge1.crossProduct(edge2);
	}

	Vector3 a, b, c;
};

#endif

