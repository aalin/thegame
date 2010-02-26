#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "player.hpp"
#include "path.hpp"
#include "sky.hpp"
#include "heightmap.hpp"
#include <chipmunk.h>

class Level
{
	public:
		Level();
		~Level();
		
		void update();
		void draw();

	private:
		Path _path;
		Sky _sky;
		Heightmap _heightmap;

		void setupSpace();
		void drawCamera();
		void drawPlayer();

		cpSpace* _space;
		cpBody* _static_body;
		cpBody* _player_body;
};

#endif

