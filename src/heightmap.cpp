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
	glVertex3f(x, y, heightAt(x, y));
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

