#include <SDL/SDL.h>
#include "cube_scene.hpp"
#include <cmath>
#include <limits>
#include <iostream>

const float PI = 3.14159265358979;
const int heightmap_size = 256;

CubeScene::CubeScene() : _heightmap(Heightmap::loadFromFile("data/heightmap.png"))
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

void lightHaxx(unsigned int light_number, float ticks, float z)
{
	float radius = std::sin(ticks / 360.0 * PI) * 50 + 25;
	float x = heightmap_size / 2 + std::cos(ticks / 360.0 * PI) * radius;
	float y = heightmap_size / 2 + std::sin(ticks / 360.0 * PI) * radius;

	float r = std::sin((ticks / 360.0) + 0.0 / 3.0 * PI);
	float g = std::sin((ticks / 360.0) + 1.0 / 3.0 * PI);
	float b = std::sin((ticks / 360.0) + 2.0 / 3.0 * PI);
	GLfloat LightAmbient[]= { 0.0, 0.0, 0.0, 1.0f };
	GLfloat LightDiffuse[]= { r, g, b, 1.0 };
	GLfloat LightPosition[]= { x, y, z, 1.0f };

	glLightfv(light_number, GL_AMBIENT, LightAmbient);
	glLightfv(light_number, GL_DIFFUSE, LightDiffuse);
	glLightfv(light_number, GL_POSITION,LightPosition);
	glEnable(light_number);

	glDisable(GL_LIGHTING);
	glPointSize(10.0);
	glBegin(GL_POINTS);
		glVertex3f(x, y, z);
	glEnd();
	glEnable(GL_LIGHTING);
}

void CubeScene::drawLights()
{
	lightHaxx(GL_LIGHT0, SDL_GetTicks() / 30.0, 32.0);
	lightHaxx(GL_LIGHT2, SDL_GetTicks() / 20.0, 44.0);
	lightHaxx(GL_LIGHT1, SDL_GetTicks() / 10.0, 32.0);
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
