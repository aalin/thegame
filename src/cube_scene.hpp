#ifndef CUBE_SCENE_HPP
#define CUBE_SCENE_HPP

#include "level.hpp"

class CubeScene
{
	public:
		CubeScene();
		void update();
		void draw();

	private:
		Level _level;
};

#endif

