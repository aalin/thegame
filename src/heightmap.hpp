#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <vector>
#include "color.hpp"

class Heightmap
{
	public:
		Heightmap(unsigned int width, unsigned int height);

		float heightAt(unsigned int x, unsigned int y)
		{
			return _heights.at(posToIndex(x, y));
		}

		void setHeightAt(unsigned int x, unsigned int y, float z)
		{
			_heights[posToIndex(x, y)] = z;
		}

		Color& colorAt(unsigned int x, unsigned int y)
		{
			return _colors.at(posToIndex(x, y));
		}

		void draw();

	protected:
		const unsigned int _width;
		const unsigned int _height;
		std::vector<float> _heights;
		std::vector<Color> _colors;

		unsigned int posToIndex(unsigned int x, unsigned int y) const
		{
			return y * _width + x;
		}
};

#endif

