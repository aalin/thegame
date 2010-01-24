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
		Vector3 edge1 = b - a;
		Vector3 edge2 = c - a;
		return edge1.crossProduct(edge2).normalize();
	}

	Vector3 a, b, c;
};

#endif

