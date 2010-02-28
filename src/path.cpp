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

struct SmoothInfo
{
	SmoothInfo(Vector3 a, Vector3 b) : a(a), b(b) { }

	const Vector3 a;
	const Vector3 b;
	Vector3 delta() { return b - a; }
	float length() { return std::fabs(delta().getMagnitude()); }
};

void Path::smoothen()
{
	// This is what I'm trying to do here.
	// http://en.wikipedia.org/wiki/B%C3%A9zier_curve#Quadratic_curves
	//
	// TODO: Higher-order curves
	std::vector<Vector3> points;
	const int detail = 10;

	for(unsigned int i = 0; i < _points.size() - 3; i += 2)
	{
		SmoothInfo info1(_points[i], _points[i + 1]);
		SmoothInfo info2(_points[i + 1], _points[i + 2]);

		for(int j = 0; j < detail; j++)
		{
			float length_into = j / static_cast<float>(detail);

			SmoothInfo inner(
				Vector3(info1.a + info1.delta().normalize() * info1.length() * length_into),
				Vector3(info1.b + info2.delta().normalize() * info2.length() * length_into)
			);

			points.push_back(inner.a + inner.delta().normalize() * inner.length() * length_into);
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

