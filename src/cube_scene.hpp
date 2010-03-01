#ifndef CUBE_SCENE_HPP
#define CUBE_SCENE_HPP

#include "player.hpp"
#include "path.hpp"
#include "sky.hpp"
#include "heightmap.hpp"
#include <chipmunk.h>

class CubeScene
{
	public:
		CubeScene();
		~CubeScene();
		void update();
		void draw();

		void keyDown(unsigned int key);
		void keyUp(unsigned int key);

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

