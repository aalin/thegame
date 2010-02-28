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
	glPointSize(5.0);
	glBegin(GL_POINTS);
		for(std::vector<Vector3>::const_iterator it = _points.begin(); it != _points.end(); it++)
			glVertex3f(it->x, it->y, it->z);
	glEnd();
}

void Path::smoothen()
{
	// This is what I'm trying to do here.
	// http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Quadratic_curves
	//
	// TODO: Higher-order curves
	std::vector<Vector3> points;
	const int detail = 10;

	for(unsigned int i = 0; i < _points.size() - 2; i += 2)
	{
		const Vector3& a = _points[i];
		const Vector3& b = _points[i+1];
		const Vector3& c = _points[i+2];

		Vector3 delta_ab(b - a);
		Vector3 delta_bc(c - b);

		float ab_length = std::fabs(delta_ab.getMagnitude());
		float bc_length = std::fabs(delta_bc.getMagnitude());

		for(int j = 0; j < detail; j++)
		{
			float length_into = j / static_cast<float>(detail);

			Vector3 one(a + delta_ab.normalize() * ab_length * length_into);
			Vector3 two(b + delta_bc.normalize() * bc_length * length_into);

			Vector3 delta(two - one);
			float new_length = std::fabs(delta.getMagnitude());

			points.push_back(one + delta.normalize() * new_length * length_into);
		}
	}

	_points = points;
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

