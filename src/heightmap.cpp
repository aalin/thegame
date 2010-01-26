#include "heightmap.hpp"
#include "triangle.hpp"
#include "opengl.hpp"
#include <iostream>

Heightmap::Heightmap(unsigned int width, unsigned int height)
	: _width(width), _height(height)
{
	_heights.resize(_width * _height);
	_colors.resize(_width * _height);
	for(size_t y = 0; y < _height; y++)
	{
		for(size_t x = 0; x < _width; x++)
		{
			colorAt(x, y) = Color(
				x * 1.0 / _width,
				y * 1.0 / _height,
				(x + y) * 1.0 / (_width + _height)
			);
		}
	}
}

void Heightmap::drawVertex(unsigned int x, unsigned int y)
{
	colorAt(x, y).draw();
	Vector3 normal(vertexNormalAt(x, y));
	glNormal3f(normal.x, normal.y, normal.z);
	Vector3 pos(positionAt(x, y));
	glVertex3f(pos.x, pos.y, pos.z);
}

Vector3 Heightmap::surfaceNormal(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	if(x0 >= _width || x1 >= _width || x2 >= _width)
		return Vector3(0.0, 0.0, 0.0);
	if(y0 >= _height || y1 >= _height || y2 >= _height)
		return Vector3(0.0, 0.0, 0.0);

	Vector3 v0 = positionAt(x0, y0);
	Vector3 v1 = positionAt(x1, y1);
	Vector3 v2 = positionAt(x2, y2);

	Vector3 n0 = Triangle(v0, v1, v2).getNormal();
	Vector3 n1 = Triangle(v2, v0, v1).getNormal();
	Vector3 n2 = Triangle(v1, v2, v0).getNormal();

	return (n0 + n1 + n2).normalize();
}

Vector3 Heightmap::vertexNormalAt(unsigned int x, unsigned int y)
{
	Vector3 a(0, 0, 0);

	a += surfaceNormal(x, y, x, y-1, x-1, y);
	a += surfaceNormal(x, y, x-1, y, x-1, y+1);
	a += surfaceNormal(x, y, x-1, y+1, x, y+1);
	a += surfaceNormal(x, y, x, y+1, x+1, y);
	a += surfaceNormal(x, y, x+1, y, x+1, y-1);
	a += surfaceNormal(x, y, x+1, y-1, x, y-1);

	return a.normalize();
}

void Heightmap::draw()
{
	for(size_t y = 0; y < _height - 1; y++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(size_t x = 0; x < _width - 1; x++)
		{
			drawVertex(x, y);
			drawVertex(x, y+1);
		}
		glEnd();
	}

	return;
	// Debug normals
	glDisable(GL_LIGHTING);
	for(size_t y = 0; y < _height - 1; y++)
	{
		for(size_t x = 0; x < _width - 1; x++)
		{
			Vector3 p1(positionAt(x, y));
			Vector3 p2(p1 + vertexNormalAt(x, y));

			glBegin(GL_LINES);
			glColor3f(0.7, 1.0, 0.7);
			glVertex3f(p1.x, p1.y, p1.z);
			glColor3f(0.7, 0.7, 1.0);
			glVertex3f(p2.x, p2.y, p2.z);
			glEnd();
		}
	}
	glEnable(GL_LIGHTING);
}

