#include <iostream>
#include <boost/assert.hpp>
#include "vector3.hpp"

void testVector3()
{
	std::cout << Vector3(3,1,2).getMagnitude() << std::endl;
	std::cout << Vector3(3,1,2).normalize() << std::endl;
}

int main()
{
	testVector3();
}
