#ifndef CUBE_SCENE_HPP
#define CUBE_SCENE_HPP

#include "opengl.hpp"
#include "heightmap.hpp"

class CubeScene
{
	public:
		CubeScene();
		void update();
		void draw();

	private:
		Heightmap _heightmap;
};

#endif

