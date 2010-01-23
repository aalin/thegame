#include "heightmap.hpp"
#include "opengl.hpp"

Heightmap::Heightmap(unsigned int width, unsigned int height)
	: _width(width), _height(height)
{
	_heights.resize(_width * _height);
	_colors.resize(_width * _height);
	for(int y = 0; y < _height; y++)
	{
		for(int x = 0; x < _width; x++)
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
	Vector3 normal(normalAt(x, y));
	glNormal3f(normal.x, normal.y, normal.z);
	Vector3 pos(positionAt(x, y));
	glVertex3f(pos.x, pos.y, pos.z);
}

Vector3 triangleNormal(const Vector3& a, const Vector3& b, const Vector3& c)
{
	Vector3 edge1 = b - a;
	Vector3 edge2 = c - a;
	return edge1.crossProduct(edge2);
}

void Heightmap::addToNormal(Vector3& normal, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	if(x0 >= _width || x1 >= _width || x2 >= _width)
		return;
	if(y0 >= _height || y1 >= _height || y2 >= _height)
		return;
	normal += triangleNormal(positionAt(x0, y0),
	                         positionAt(x1, y1),
	                         positionAt(x2, y2));
}

// I have no idea if I'm doing this right. :D
// I'm trying to do this: http://www.gamedev.net/community/forums/topic.asp?topic_id=78633
Vector3 Heightmap::normalAt(unsigned int x, unsigned int y)
{
	Vector3 normal;
	addToNormal(normal, x, y, x-1, y+0, x-1, y+1);
	addToNormal(normal, x, y, x-1, y+1, x+0, y+1);
	addToNormal(normal, x, y, x+0, y+1, x+1, y+0);
	addToNormal(normal, x, y, x+1, y+0, x+1, y-1);
	addToNormal(normal, x, y, x+1, y-1, x+0, y-1);
	addToNormal(normal, x, y, x+0, y-1, x-1, y+0);
	return normal.normalize();
}

void Heightmap::draw()
{
	glBegin(GL_TRIANGLE_STRIP);

	for(int y = 0; y < _height - 1; y++)
	{
		if(y % 2 == 0) // Even, left to right
		{
			for(int x = 0; x < _width - 1; x++)
			{
				drawVertex(x, y);
				drawVertex(x+1, y);
				drawVertex(x, y+1);
				drawVertex(x+1, y+1);
			}
		}
		else // Odd, right to left
		{
			for(int x = _width - 1; x > 0; x--)
			{
				drawVertex(x, y);
				drawVertex(x-1, y);
				drawVertex(x, y+1);
				drawVertex(x-1, y+1);
			}
		}
	}
	glEnd();
}

