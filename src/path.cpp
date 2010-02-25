#include "path.hpp"
#include "opengl.hpp"
#include <cmath>

void Path::draw() const
{
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);

	for(std::vector<Vector3>::const_iterator it = _points.begin(); it != _points.end(); it++)
		glVertex3f(it->x, it->y, it->z);

	glEnd();
}

float Path::length() const
{
	float len = 0.0;

	for(unsigned int i = 0; i < _points.size() - 1; i++)
	{
		const Vector3& current = _points[i];
		const Vector3& next    = _points[i+1];

		len += std::fabs((next - current).getMagnitude());
	}

	return len;
}

Vector3 Path::positionAt(float length) const
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

		if(length >= length_so_far && length < length_so_far + line_length)
		{
			float length_into_current = length - length_so_far;
			return current + delta.normalize() * length_into_current;
		}

		length_so_far += line_length;
	}

	return _points.front();
}

