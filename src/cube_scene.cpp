#include <SDL/SDL.h>
#include "cube_scene.hpp"
#include <cmath>
#include <limits>
#include <iostream>

const float PI = 3.14159265358979;
const int heightmap_size = 256;

CubeScene::CubeScene() : _heightmap(Heightmap::loadFromFile("data/heightmap.rgba"))
{
}

void CubeScene::update()
{
	_heightmap.update();
}

void drawCamera()
{
	float ticks = SDL_GetTicks() / 30.0;
	float distance = heightmap_size / 2;
	float x = std::cos(ticks / 180.0 * PI) * distance;
	float y = std::sin(ticks / 180.0 * PI) * distance;
	float z = 100; //distance/3*2 + std::sin(ticks / 320.0 * PI) * distance / 2;

	gluLookAt(
		x+heightmap_size/2, y+heightmap_size/2, z,
		heightmap_size/2, heightmap_size/2, 50.0,
		0.0, 0.0, 1.0
	);
}

void CubeScene::drawLights()
{
	float ticks = SDL_GetTicks() / 30.0;
	float r = std::sin(ticks / 360.0 * PI) * 50 + 25;
	float x = heightmap_size / 2 + std::cos(ticks / 360.0 * PI) * r;
	float y = heightmap_size / 2 + std::sin(ticks / 360.0 * PI) * r;
	float z = 32.0;
	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]= { x, y, z, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);

	glDisable(GL_LIGHTING);
	glPointSize(10.0);
	glBegin(GL_POINTS);
		glVertex3f(x, y, z);
	glEnd();
	glEnable(GL_LIGHTING);
}

void CubeScene::draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 800.0 / 600.0, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawCamera();

	drawLights();
	_heightmap.draw();
}
