#include <SDL/SDL.h>
#include "cube_scene.hpp"
#include <cmath>
#include <limits>
#include <iostream>

const float PI = 3.14159265358979;

CubeScene::CubeScene()
{ }

void CubeScene::update()
{ }

void drawCamera()
{
	float ticks = SDL_GetTicks() / 10.0;
	float distance = 40; //15.0 + std::sin(ticks / 180.0 * PI) * 5.0;
	float x = std::cos(ticks / 180.0 * PI) * distance;
	float y = std::sin(ticks / 320.0 * PI) * distance;
	float z = std::sin(ticks / 180.0 * PI) * distance;

	gluLookAt(
		x, y, z,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);
}

void drawThing()
{
	glLineWidth(2);

	glBegin(GL_LINE_STRIP);
		const int total = 2000;
		for(int i = 0; i < total; i++)
		{
			float a = i * 1.0 / total;
			glColor4f(
				std::sin((a + 0 / 3.0) * 2 * PI) / 2.0 + 0.5,
				std::sin((a + 1 / 3.0) * 2 * PI) / 2.0 + 0.5,
				std::sin((a + 2 / 3.0) * 2 * PI) / 2.0 + 0.5,
				std::sin((1.0 - a) * PI)
			);
			
			float ysad = (SDL_GetTicks() / 1000.0);
			float x = std::cos(a * 100 * PI) * i / 100.0;
			float y = std::sin(a * ysad * PI) * 10;
			float z = std::sin(a * 100 * PI) * i / 100.0;
			glVertex3f(x, y, z);
		}
	glEnd();
}

void CubeScene::draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 800.0 / 600.0, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawCamera();
	drawThing();
}
