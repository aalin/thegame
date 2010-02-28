#include "heightmap.hpp"
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

	for(unsigned int i = 0; i < _points.size() - 2; i++)
	{
		SmoothInfo info(_points.at(i), _points.at(i + 1));

		for(int j = 0; j < detail; j++)
		{
			float length_into = j / static_cast<float>(detail);

			points.push_back(info.a + info.delta().normalize() * info.length() * length_into);
		}
	}

	_points = points;
}

void Path::setHeightsFromHeightmap(const Heightmap& heightmap)
{
	for(unsigned int i = 0; i < _points.size(); i++)
		_points[i].z = heightmap.interpolatedHeightAt(_points[i].x, _points[i].y);
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

