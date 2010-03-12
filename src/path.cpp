#include "heightmap.hpp"
#include "path.hpp"
#include "opengl.hpp"
#include <cmath>
#include <iostream>

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
	std::vector<Vector3> points;

	unsigned int smooth_length = 30;
	float len = length();
	for(float i = 0.0; i < len - smooth_length - 1; i += 1)
	{
		Vector3 a(positionAt(i));

		Vector3 normal;
		for(unsigned int j = 0; j < smooth_length; j++)
			normal += SmoothInfo(a, positionAt(i + j)).delta().normalize() * (j / static_cast<float>(smooth_length));

		SmoothInfo info(a, positionAt(i + smooth_length));
		Vector3 v(info.a + normal.normalize() * info.length() * 0.5);
		points.push_back(v);
	}

	_points = points;
}

Vector3 Path::normalAt(float position) const
{
	return (
		positionAt(position + 1.0) * Vector3(1.0, 1.0, 0.0) -
		positionAt(position)       * Vector3(1.0, 1.0, 0.0)
	).normalize();
}

float Path::angleAt(float position) const
{
	Vector3 normal(normalAt(position));
	float angle = std::acos(normal.x) * 180 / M_PI;
	return (normal.y < 0) ? 360 - angle : angle;
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

