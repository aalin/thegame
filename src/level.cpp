#include "level.hpp"
#include <cmath>
#include <iostream>

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

	setupSpace();
}

void Level::setupSpace()
{
	_space = cpSpaceNew();
	_space->iterations = 10;
	_space->gravity = cpv(0, -5);
	
	_static_body = cpBodyNew(INFINITY, INFINITY);

	cpShape* shape;
	for(unsigned int i = 0; i < _path.length() - 1; i++)
	{
		Vector3 pos1(_path.positionAt(i));
		Vector3 pos2(_path.positionAt(i + 1));
		shape = cpSpaceAddStaticShape(_space, cpSegmentShapeNew(_static_body, cpv(i, pos1.z), cpv(i + 1, pos2.z), 0.0));
		shape->e = 1.0;
		shape->u = 1.0;
	}

	// Player

	_player_body = cpSpaceAddBody(_space, cpBodyNew(10.0, INFINITY));
	_player_body->p = cpv(50, 50);

	shape = cpSpaceAddShape(_space, cpCircleShapeNew(_player_body, 1.0, cpvzero));
}

Level::~Level()
{
	cpBodyFree(_static_body);
	cpBodyFree(_player_body);
	cpSpaceFreeChildren(_space);
	cpSpaceFree(_space);
}

void Level::update()
{
	for(int i = 0; i < 2; i++)
		cpSpaceStep(_space, 1.0 / 60.0 / 2);

	_heightmap.update();
}

void drawLights()
{
	float ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	float diffuse[] = { 1.0, 1.0, 1.0, 0.5 };
	float pos[]     = { 0.0, 0.0, 100, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);
}

void drawMaterial()
{
	float ambient[]  = { 1.00, 1.00, 1.00, 1.00 };
	float diffuse[]  = { 0.43, 0.47, 0.54, 1.00 };
	float specular[] = { 0.33, 0.33, 0.53, 1.00 };
	float emission[] = { 0.0, 0.0, 0.0, 0.0};

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

void Level::drawCamera()
{
	Vector3 position(_path.positionAt(_player_body->p.x));
	position.z = _player_body->p.y;

	Vector3 next(_path.positionAt(_player_body->p.x + 5.0));
	next.z = _player_body->p.y;

	Vector3 normal((next - position).normalize());

	gluLookAt(
		position.x + normal.x * -80, position.y + normal.y * -80, 40,
		position.x, position.y, position.z,
		0.0, 0.0, 1.0
	);
}

void Level::drawPlayer()
{
	Vector3 pos(_path.positionAt(_player_body->p.x));
	pos.z = _player_body->p.y;

	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex3f(pos.x, pos.y, pos.z);
	glEnd();
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

	drawCamera();

	_sky.draw();
	drawLights();
	drawFog();

	drawMaterial();

	_heightmap.draw();
	_path.draw();

	drawPlayer();
}

