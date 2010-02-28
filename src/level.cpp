#include "level.hpp"
#include <cmath>

Level::Level()
	: _sky(256),
	  _heightmap(Heightmap::loadFromFile("data/asd"))
{
	unsigned int num_points = 40;
	for(unsigned int i = 0; i < num_points; i++)
	{
		float x = std::cos(i * 360.0 / num_points / 180.0 * M_PI) * 128 / 4.0 + 128 / 2;
		float y = std::sin(i * 360.0 / num_points / 180.0 * M_PI) * 128 / 4.0 + 128 / 2;
		float z = _heightmap.heightAt(x, y) + 4.0;
		_path.addPoint(x, y, z);
	}
	_path.smoothen();
	_path.setHeightsFromHeightmap(_heightmap);
	_player.setPath(_path);
}

void Level::update()
{
	_heightmap.update();
	_player.update();
}

void drawLights()
{
	float ambient[]= { 0.0, 0.0, 0.0, 1.0f };
	float diffuse[]= { 1.0, 1.0, 1.0, 0.5 };
	float position[]= { 0, 0, 100, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
}

void drawMaterial()
{
	float ambient[] = {1, 1, 1, 1.0};
	float diffuse[] = {0.43, 0.47, 0.54, 1.0};
	float specular[] = {0.33, 0.33, 0.53, 1.0};
	float emission[] = {0.0, 0.0, 0.0, 0.0};
	float shininess = 10.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
}

void drawFog()
{
    float color[] = {0.0, 0.0, 0.0, 0.5};
    glFogfv(GL_FOG_COLOR, color);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_DENSITY, 0.1);
    glHint(GL_FOG_HINT, GL_NICEST);
    glFogf(GL_FOG_START, 300.0);
    glFogf(GL_FOG_END, 500.0);
}

void drawCamera(const Player& player)
{
	Vector3 player_position = player.positionAt();

	Vector3 player_direction;
	player_direction += (player.positionAt(1.0) - player_position).normalize();
	player_direction += (player.positionAt(3.0) - player_position).normalize() * 0.75;
	player_direction += (player.positionAt(5.0) - player_position).normalize() * 0.50;
	player_direction += (player.positionAt(7.0) - player_position).normalize() * 0.25;
	player_direction += (player.positionAt(10.0) - player_position).normalize() * 0.25;
	player_direction += (player.positionAt(15.0) - player_position).normalize() * 0.25;
	player_direction.normalize();

	gluLookAt(
		player_position.x + player_direction.x * -80, player_position.y + player_direction.y * -80, 40,
		player_position.x, player_position.y, player_position.z,
		0.0, 0.0, 1.0
	);
}

void Level::draw()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_FOG);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 800.0 / 600.0, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawCamera(_player);

	_sky.draw();
	drawLights();
	drawFog();

	drawMaterial();

	_heightmap.draw();
	_player.draw();
}

