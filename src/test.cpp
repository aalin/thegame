#include <iostream>
#include <cmath>
#include <boost/assert.hpp>
#include "vector3.hpp"

void testVector3()
{
	BOOST_ASSERT(Vector3(3,1,2).getMagnitude() == 14);
	BOOST_ASSERT(Vector3(3,1,2).normalize() == Vector3(3 / std::sqrt(14), 1 / std::sqrt(14), 2 / std::sqrt(14)));
}

int main()
{
	testVector3();
}
