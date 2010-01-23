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

void Heightmap::draw()
{
	for(int y = 0; y < _height - 1; y++)
	{
		glBegin(GL_QUADS);
		for(int x = 0; x < _width - 1; x++)
		{
			colorAt(x, y).draw();
			glVertex3f(x, y, heightAt(x, y));
			colorAt(x+1, y).draw();
			glVertex3f(x+1, y, heightAt(x+1, y));
			colorAt(x+1, y+1).draw();
			glVertex3f(x+1, y+1, heightAt(x+1, y+1));
			colorAt(x, y+1).draw();
			glVertex3f(x, y+1, heightAt(x, y+1));
		}
		glEnd();
	}
}

