#include "path.hpp"
#include "opengl.hpp"
#include <boost/foreach.hpp>
#include <cmath>

void Path::draw()
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);

	BOOST_FOREACH(const Vector3& v, _points)
		glVertex3f(v.x, v.y, v.z);

	glEnd();
}

float Path::length()
{
	float len = 0.0;
	const Vector3* previous = 0;

	BOOST_FOREACH(const Vector3& v, _points)
	{
		if(previous)
			len += std::fabs((v - *previous).getMagnitude());
		previous = &v;
	}

	return len;
}

// TODO: Figure out the current position at the given length, counting from the beginning, and return it.
Vector3 Path::positionAt(float length)
{
	return _points.front();
}

