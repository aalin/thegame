#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <vector>
#include "color.hpp"
#include "vector3.hpp"

class Heightmap
{
	public:
		Heightmap(unsigned int width, unsigned int height);

		float heightAt(unsigned int x, unsigned int y) const
		{
			return _heights.at(posToIndex(x, y));
		}

		void setHeightAt(unsigned int x, unsigned int y, float z)
		{
			_heights[posToIndex(x, y)] = z;
		}

		Vector3 positionAt(unsigned int x, unsigned int y) const
		{
			return Vector3(x, y, heightAt(x, y));
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

		void drawVertex(unsigned int x, unsigned int y);
		Vector3 normalAt(unsigned int x, unsigned int y);

		unsigned int posToIndex(unsigned int x, unsigned int y) const
		{
			return y * _width + x;
		}

		Vector3 surfaceNormal(
			unsigned int x0, unsigned int y0,
			unsigned int x1, unsigned int y1,
			unsigned int x2, unsigned int y2);
};

#endif

