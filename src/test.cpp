#include <iostream>
#include <cmath>
#include <boost/assert.hpp>
#include "vector3.hpp"
#include "triangle.hpp"
#include "heightmap.hpp"

#define ASSERT_EQUAL_FLOATS(a, b) BOOST_ASSERT(std::fabs(a - b) < std::numeric_limits<float>::epsilon())

void testVector3()
{
	ASSERT_EQUAL_FLOATS(Vector3(3,1,2).getMagnitude(), std::sqrt(14));
	BOOST_ASSERT(Vector3(3,1,2).normalize() == Vector3(3 / std::sqrt(14), 1 / std::sqrt(14), 2 / std::sqrt(14)));
	BOOST_ASSERT( Vector3(1, 2, 3).crossProduct(Vector3(4, 5, 6)) == Vector3(-3, 6, -3) );

	// http://www.wolframalpha.com/input/?i={1%2F4%2C+-1%2F2%2C+1}+cross+{1%2F3%2C+1%2C+-2%2F3}
	Vector3 a(1/4.0, -1/2.0, 1.0);
	Vector3 b(1/3.0, 1.0, -2/3.0);
	BOOST_ASSERT( Vector3(1/4.0, -1/2.0, 1.0).crossProduct(Vector3(1/3.0, 1.0, -2/3.0)) == Vector3(-2/3.0, 1/2.0, 5/12.0) );
}

void testTheThing()
{
	Vector3 a(2, 1, 0);
	Vector3 b(5, 3, 0);
	Vector3 c = b - a;

	float distance = c.getMagnitude();

	BOOST_ASSERT(a + c.normalize() * distance == b);
}

void testTriangle()
{
	BOOST_ASSERT(
		Triangle(
			Vector3(0, 0, 0),
			Vector3(1, 1, 0),
			Vector3(-1, 1, 0)
		).getNormal() == Vector3(0, 0, 1)
	);
}

void testNormals()
{
	const int w = 4;
	const int h = 4;
	Heightmap hmap(w, h);
	for(int y = 0; y < h; y++)
		for(int x = 0; x < w; x++)
			hmap.setHeightAt(x, y, y * w + x);
	// TODO: Test normals
}

int main()
{
	testTheThing();
	testVector3();
	testTriangle();
	testNormals();
	return 0;
}
