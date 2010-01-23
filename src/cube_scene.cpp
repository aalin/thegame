#include <SDL/SDL.h>
#include "cube_scene.hpp"
#include <cmath>

const float PI = 3.14159265358979;

CubeScene::CubeScene()
{
	_i = 0;
}

void CubeScene::update()
{
	_i += 1;
}

void drawCamera()
{
	float ticks = SDL_GetTicks() / 10.0;
	float distance = 40; //15.0 + std::sin(ticks / 180.0 * PI) * 5.0;
	float x = std::cos(ticks / 180.0 * PI) * distance;
	float y = 10.0; //std::sin(ticks / 320.0 * PI) * distance;
	float z = std::sin(ticks / 180.0 * PI) * distance;

	gluLookAt(
		x, y, z,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);
}

void drawCube()
{
	glBegin(GL_QUADS);
		glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Green
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)
		glColor3f(1.0f,0.5f,0.0f);			// Set The Color To Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Bottom)
		glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Front)
		glColor3f(1.0f,1.0f,0.0f);			// Set The Color To Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Back)
		glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Left)
		glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Violet
		glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Right)
		glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Right)
		glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Right)
	glEnd();
}

void drawThing()
{
	glBegin(GL_LINE_STRIP);
		const int total = 2000;
		for(int i = 0; i < total; i++)
		{
			float a = i * 1.0 / total;
			glColor3f(
				std::sin((a + 0 / 3.0) * 2 * PI) / 2.0 + 0.5,
				std::sin((a + 1 / 3.0) * 2 * PI) / 2.0 + 0.5,
				std::sin((a + 2 / 3.0) * 2 * PI) / 2.0 + 0.5
			);

			float x = std::cos(a * 100 * PI) * i / 100.0;
			float y = std::sin(a * 120 * PI) * i / 100.0;
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
	drawCube();
	drawThing();
}
