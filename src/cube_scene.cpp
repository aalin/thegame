#include <SDL/SDL.h>
#include "cube_scene.hpp"

CubeScene::CubeScene()
{
}

void CubeScene::update()
{
	_level.update();
}

void CubeScene::draw()
{
	_level.draw();
}
