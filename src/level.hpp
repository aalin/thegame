#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "player.hpp"
#include "path.hpp"
#include "sky.hpp"
#include "heightmap.hpp"

class Level
{
	public:
		Level();
		
		void update();
		void draw();

	private:
		Player _player;
		Path _path;
		Sky _sky;
		Heightmap _heightmap;
};

#endif

