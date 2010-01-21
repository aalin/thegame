#include "heightmap.hpp"

Heightmap::Heightmap(unsigned int width, unsigned int height)
	: _width(width), _height(height)
{
	_data.resize(_width * _height, 0.0);
}

