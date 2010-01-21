#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <vector>

class Heightmap
{
	public:
		Heightmap(unsigned int width, unsigned int height);

		float at(unsigned int x, unsigned int y) const
		{
			return _data[y * _width + x];
		}

		void set(unsigned int x, unsigned int y, float value)
		{
			_data[y * _width + x] = value;
		}

	protected:
		const unsigned int _width;
		const unsigned int _height;
		std::vector<float> _data;
};

#endif

