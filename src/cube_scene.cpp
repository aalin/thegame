#include <SDL/SDL.h>
#include "cube_scene.hpp"
#include <cmath>
#include <limits>
#include <iostream>

const float PI = 3.14159265358979;
const int heightmap_size = 256;

CubeScene::CubeScene()
	: _sky(heightmap_size + 30),
	  _heightmap(Heightmap::loadFromFile("data/heightmap.png"))
{
	_player_path_pos = 0;

	unsigned int num_points = 40;
	for(unsigned int i = 0; i < num_points; i++)
	{
		float x = std::cos(i * 360.0 / num_points / 180.0 * PI) * heightmap_size / 4.0 + heightmap_size / 2;
		float y = std::sin(i * 360.0 / num_points / 180.0 * PI) * heightmap_size / 4.0 + heightmap_size / 2;
		float z = _heightmap.heightAt(x, y) + 4.0;
		_path.addPoint(x, y, z);
	}
}

void CubeScene::update()
{
	_heightmap.update();
	_player_path_pos += 0.05;
}

Vector3 cameraPosAt(float ticks)
{
	Vector3 pos(
		std::cos(ticks * 2.0 / 180.0 * PI) * heightmap_size / 2.5 + std::sin(ticks * 10.0 / 180.0 * PI) * heightmap_size / 16,
		std::sin(ticks * 2.0 / 180.0 * PI) * heightmap_size / 2.5 + std::cos(ticks * 10.0 / 180.0 * PI) * heightmap_size / 16,
		0.0
	);
	return pos;
}

void drawCamera()
{
	float ticks = SDL_GetTicks() / 100.0;

	Vector3 origin = Vector3(heightmap_size / 2.0, heightmap_size / 2.0, 0.0);
	Vector3 eye_pos = cameraPosAt(ticks) + origin;
	Vector3 look_at = cameraPosAt(ticks + 30.0);
	eye_pos.z = 40;
	look_at *= 0.5;
	look_at += origin;
	look_at.z = 30;

	gluLookAt(
		eye_pos.x, eye_pos.y, eye_pos.z,
		look_at.x, look_at.y, look_at.z,
		0.0, 0.0, 1.0
	);
}

void lightHaxx(unsigned int light_number, float ticks, float z)
{
	float radius = std::sin(ticks / 360.0 * PI) * 50 + 25;
	float x = heightmap_size / 2 + std::cos(ticks / 360.0 * PI) * radius;
	float y = heightmap_size / 2 + std::sin(ticks / 360.0 * PI) * radius;

	GLfloat LightAmbient[]= { 0.0, 0.0, 0.0, 1.0f };
	GLfloat LightDiffuse[]= { 1.0, 1.0, 1.0, 0.5 };
	GLfloat LightPosition[]= { x, y, z, 1.0f };

	glLightfv(light_number, GL_AMBIENT, LightAmbient);
	glLightfv(light_number, GL_DIFFUSE, LightDiffuse);
	glLightfv(light_number, GL_POSITION,LightPosition);
	glEnable(light_number);

	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glVertex3f(x, y, 0.0);
		glVertex3f(x, y, 100.0);
	glEnd();
	glDisable(GL_FOG);
	glPointSize(10.0);
	glBegin(GL_POINTS);
		glVertex3f(x, y, z);
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_FOG);
}

void CubeScene::drawLights()
{
	lightHaxx(GL_LIGHT0, SDL_GetTicks() / 30.0, 50.0);
	lightHaxx(GL_LIGHT0, SDL_GetTicks() / 10.0, 50.0);
}

void CubeScene::drawFog()
{
    GLfloat fog_color[] = {0.0, 0.0, 0.0, 0.5};
    glFogfv(GL_FOG_COLOR, fog_color);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_DENSITY, 0.1);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogf(GL_FOG_START, 300.0);
    glFogf(GL_FOG_END, 500.0);

	glEnable(GL_FOG);
}

void CubeScene::drawMaterial()
{
    GLfloat material_ka[] = {1, 1, 1, 1.0};
    GLfloat material_kd[] = {0.43, 0.47, 0.54, 1.0};
    GLfloat material_ks[] = {0.33, 0.33, 0.53, 1.0};
    GLfloat material_ke[] = {0.0, 0.0, 0.0, 0.0};
    GLfloat material_se = 10;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_ks);
    glMaterialfv(GL_FRONT, GL_EMISSION, material_ke);
    glMaterialfv(GL_FRONT, GL_SHININESS, &material_se);
	glEnable(GL_COLOR_MATERIAL);
}

void CubeScene::draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 800.0 / 600.0, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3 player_position(_path.positionAt(_player_path_pos));

	// drawCamera();
	gluLookAt(
		heightmap_size / 2, heightmap_size / 2, 40,
		player_position.x, player_position.y, player_position.z,
		0.0, 0.0, 1.0
	);

	glPushMatrix();
		glTranslatef(heightmap_size / 2, heightmap_size / 2, -20.0);
		_sky.draw();
	glPopMatrix();

	drawLights();
	drawFog();
	drawMaterial();
	_heightmap.draw();

	_path.draw();

	glDisable(GL_LIGHTING);
	glPointSize(50.0);
	glBegin(GL_POINTS);
		glVertex3f(player_position.x, player_position.y, player_position.z);
	glEnd();
	glEnable(GL_LIGHTING);
}
