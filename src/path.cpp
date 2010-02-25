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
	// Make the length fit.
	float total_length = this->length();
	while(length > total_length)
		length -= total_length;

	float length_so_far = 0.0;

	for(unsigned int i = 0; i < _points.size() - 1; i++)
	{
		const Vector3& current = _points[i];
		const Vector3& next    = _points[i+1];

		Vector3 delta(next - current);

		float line_length = std::fabs(delta.getMagnitude());

		// The given length is between current and next
		if(length >= length_so_far && length < length_so_far + line_length)
		{
			float length_into_current = length - length_so_far;
			return current + delta.normalize() * length_into_current;
		}

		length_so_far += line_length;
	}
	return _points.front();
}

