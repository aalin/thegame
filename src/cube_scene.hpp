#ifndef CUBE_SCENE_HPP
#define CUBE_SCENE_HPP

#include "opengl.hpp"
#include "heightmap.hpp"
#include "sky.hpp"

class CubeScene
{
	public:
		CubeScene();
		void update();
		void draw();

	private:
		Sky _sky;
		Heightmap _heightmap;
		void drawLights();
		void drawFog();
		void drawMaterial();
};

#endif

