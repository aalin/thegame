#include <SDL/SDL.h>
#include "cube_scene.hpp"
#include <cmath>
#include <limits>
#include <iostream>

const float PI = 3.14159265358979;

CubeScene::CubeScene() : _heightmap(128, 128)
{
	for(int i = 0; i < 1280; i++)
	{
		float r = i / 20.0;
		for(int j = 0; j < 360*2; j++)
		{
			float x = 64 + std::cos(j / 180.0 / 2 * PI) * r;
			float y = 64 + std::sin(j / 180.0 / 2 * PI) * r;
			float z = std::sin(i / 100.0 * PI);
			if(x < 128 && y < 128 && x > 0 && y > 0)
				_heightmap.setHeightAt(x, y, z);
		}
	}
}

void CubeScene::update()
{ }

void drawCamera()
{
	float ticks = SDL_GetTicks() / 30.0;
	float distance = 100;
	float x = std::cos(ticks / 180.0 * PI) * distance;
	float y = std::sin(ticks / 180.0 * PI) * distance;
	float z = distance/3*2 + std::sin(ticks / 320.0 * PI) * distance / 2;

	gluLookAt(
		x+64, y+64, z,
		64.0, 64.0, 0.0,
		0.0, 0.0, 1.0
	);
}

void CubeScene::draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 800.0 / 600.0, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawCamera();
	_heightmap.draw();
}
