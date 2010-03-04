#include "cube_scene.hpp"
#include <SDL/SDL.h>
#include <cmath>
#include <iostream>

CubeScene::CubeScene()
	: _sky(256),
	  _heightmap(Heightmap::loadFromFile("data/asd"))
{
	_path.addPoint(97, 113.0, 0.0);
	_path.addPoint(93, 119.5, 0.0);
	_path.addPoint(85, 122.0, 0.0);
	_path.addPoint(71, 116.0, 0.0);
	_path.addPoint(68, 107.0, 0.0);
	_path.addPoint(89, 94.5, 0.0);
	_path.addPoint(83, 89.0, 0.0);
	_path.addPoint(75, 59.0, 0.0);
	_path.addPoint(91, 43.5, 0.0);
	_path.addPoint(67, 24.5, 0.0);
	_path.addPoint(69, 13.5, 0.0);
	_path.addPoint(63, 7.5, 0.0);
	_path.addPoint(93, 4.0, 0.0);
	_path.addPoint(108, 12.5, 0.0);
	_path.addPoint(138, 9.5, 0.0);
	_path.addPoint(143, 18.0, 0.0);
	_path.addPoint(154, 22.5, 0.0);
	_path.addPoint(157, 26.0, 0.0);
	_path.addPoint(169, 28.0, 0.0);
	_path.addPoint(170, 32.0, 0.0);
	_path.addPoint(158, 34.5, 0.0);
	_path.addPoint(151, 46.0, 0.0);
	_path.addPoint(174, 52.0, 0.0);
	_path.addPoint(193, 45.0, 0.0);
	_path.addPoint(215, 19.5, 0.0);
	_path.addPoint(226, 18.0, 0.0);
	_path.addPoint(232, 25.5, 0.0);
	_path.addPoint(227, 38.5, 0.0);
	_path.addPoint(243, 46.0, 0.0);
	_path.addPoint(245, 51.0, 0.0);
	_path.addPoint(231, 55.5, 0.0);
	_path.addPoint(230, 60.5, 0.0);
	_path.addPoint(241, 65.5, 0.0);
	_path.addPoint(239, 74.0, 0.0);
	_path.addPoint(213, 84.5, 0.0);
	_path.addPoint(213, 88.5, 0.0);
	_path.addPoint(197, 96.0, 0.0);
	_path.addPoint(214, 102.5, 0.0);
	_path.addPoint(217, 118.0, 0.0);
	_path.addPoint(210, 120.0, 0.0);
	_path.addPoint(177, 106.0, 0.0);
	_path.addPoint(163, 104.0, 0.0);
	_path.addPoint(155, 100.0, 0.0);
	_path.addPoint(161, 91.0, 0.0);
	_path.addPoint(180, 78.5, 0.0);
	_path.addPoint(181, 72.5, 0.0);
	_path.addPoint(171, 68.0, 0.0);

	_path.smoothen();
	_path.setHeightsFromHeightmap(_heightmap);

	setupSpace();

	_arrow_direction = cpv(0, 0);
	_last_jump_state = false;
}

void playerUpdateVelocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
	cpBodyUpdateVelocity(body, gravity, damping, dt);
	body->v.x = cpfclamp(body->v.x, -5, 5);
	body->v.y = cpfmax(body->v.y, -5);
}

int begin(cpArbiter *arb, cpSpace* space, void* ignore)
{
	CP_ARBITER_GET_SHAPES(arb, a, b);
	CubeScene* cube_scene = static_cast<CubeScene*>(a->data);

	cpVect n = cpvneg(cpArbiterGetNormal(arb, 0));
	if(n.y > 0.0)
		cpArrayPush(cube_scene->_player_ground_shapes, b);

	return 1;
}

int preSolve(cpArbiter *arb, cpSpace* space, void* ignore)
{
	CP_ARBITER_GET_SHAPES(arb, a, b);
	CubeScene* cube_scene = static_cast<CubeScene*>(a->data);

	if(arb->stamp > 0)
	{
		a->u = 1.0;

		cpVect n = cpvneg(cpArbiterGetNormal(arb, 0));
		if(n.y > cube_scene->_player_ground_normal.y)
			cube_scene->_player_ground_normal = n;
	}

	return 1;
}

void separate(cpArbiter *arb, cpSpace* space, void* ignore)
{
	CP_ARBITER_GET_SHAPES(arb, a, b);
	CubeScene* cube_scene = static_cast<CubeScene*>(a->data);

	cpArrayDeleteObj(cube_scene->_player_ground_shapes, b);

	if(cube_scene->_player_ground_shapes->num == 0)
		a->u = 0.0;
}

void addGroundShape(cpSpace* space, cpBody* body, cpVect a, cpVect b)
{
	cpShape* shape = cpSpaceAddStaticShape(space, cpSegmentShapeNew(body, a, b, 0.0));
	shape->e = 1.0;
	shape->u = 1.0;
	shape->collision_type = 2;
}

void CubeScene::setupSpace()
{
	_space = cpSpaceNew();
	_space->iterations = 10;
	_space->gravity = cpv(0, -300);
	
	_static_body = cpBodyNew(INFINITY, INFINITY);

	for(unsigned int i = 0; i < _path.length() - 1; i++)
	{
		Vector3 pos1(_path.positionAt(i));
		Vector3 pos2(_path.positionAt(i + 1));
		addGroundShape(_space, _static_body, cpv(i, pos1.z), cpv(i + 1, pos2.z));
	}

	// Player

	_player_body = cpSpaceAddBody(_space, cpBodyNew(10.0, INFINITY));
	_player_body->p = cpv(20.0, _path.positionAt(20.0).z + 1);
	_player_body->velocity_func = &playerUpdateVelocity;

	_player_shape = cpSpaceAddShape(_space, cpCircleShapeNew(_player_body, 1.0, cpvzero));
	_player_shape->e = 0.0;
	_player_shape->u = 2.0;
	_player_shape->collision_type = 1;
	_player_shape->data = this;

	_player_ground_shapes = cpArrayNew(0);

	cpSpaceAddCollisionHandler(_space, 1, 2, &begin, &preSolve, 0, 0, this);
}

CubeScene::~CubeScene()
{
	cpBodyFree(_static_body);
	cpBodyFree(_player_body);
	cpSpaceFreeChildren(_space);
	cpSpaceFree(_space);
}

void CubeScene::update()
{
	if(_player_ground_normal.y > 0.0)
		_player_shape->surface_v = cpvmult(cpvperp(_player_ground_normal), 200 * _arrow_direction.x);
	else
		_player_shape->surface_v = cpvzero;

	bool jump_state = _arrow_direction.y > 0;

	if(jump_state && !_last_jump_state && cpvlengthsq(_player_ground_normal))
	{
 		_player_body->v = cpvadd(_player_body->v, cpvmult(cpvslerp(_player_ground_normal, cpv(0.0, 1.0), 0.75), 50));
	}

	if(_player_ground_shapes->num == 0)
	{
		float air_accel = _player_body->v.x + _arrow_direction.x * 20.0;
		_player_body->f.x = _player_body->m * air_accel;
	}

	_last_jump_state = jump_state;
	_player_ground_normal = cpvzero;

	for(int i = 0; i < 3; i++)
		cpSpaceStep(_space, 1.0 / 60.0 / 3);

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

void CubeScene::drawCamera()
{
	Vector3 position(_path.positionAt(_player_body->p.x));
	position.z = _player_body->p.y;

	Vector3 next(_path.positionAt(_player_body->p.x + 1.0));
	next.z = _player_body->p.y;

	Vector3 normal;
	for(int i = 0; i < 20; i++)
		normal += (_path.positionAt(_player_body->p.x + i / 20.0 * 8.0) * Vector3(1.0, 1.0, 0.0) + Vector3(0.0, 0.0, _player_body->p.y) - position).normalize();
	normal.normalize();

	Vector3 camera_pos(
		position.x + normal.x * -20,
		position.y + normal.y * -20,
		0.0
	);

	float height_at_pos = _heightmap.interpolatedHeightAt(camera_pos.x, camera_pos.y);
	camera_pos.z = height_at_pos + 10.0;

	// Temporary, until the heightmap is properly unmirrored.
	camera_pos.x = -5.0;
	camera_pos.y = -5.0;
	camera_pos.z = 50.0;

	gluLookAt(
		camera_pos.x, camera_pos.y, camera_pos.z,
		position.x, position.y, position.z,
		0.0, 0.0, 1.0
	);
}

void CubeScene::drawPlayer()
{
	Vector3 pos(_path.positionAt(_player_body->p.x));
	pos.z = _player_body->p.y;

	glPointSize(5.0);
	glBegin(GL_POINTS);
	glVertex3f(pos.x, pos.y, pos.z);
	glEnd();
}

void CubeScene::draw()
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

	glPushMatrix();
		glTranslatef(_heightmap.width() / 2.0, _heightmap.height() / 2.0, -40);
		_sky.draw();
	glPopMatrix();
	drawLights();
	drawFog();

	drawMaterial();

	_heightmap.draw();
	_path.draw();

	drawPlayer();
}

void CubeScene::updateArrowDirection(unsigned int key, int value)
{
	switch(key)
	{
		case SDLK_UP:
			_arrow_direction.x += value;
			break;
		case SDLK_DOWN:
			_arrow_direction.x -= value;
			break;
		case SDLK_SPACE:
			_arrow_direction.y += value;
			 break;
	}
}

void CubeScene::keyDown(unsigned int key)
{
	std::cout << "Key down: " << key << std::endl;
	updateArrowDirection(key, +1);
}

void CubeScene::keyUp(unsigned int key)
{
	std::cout << "Key up: " << key << std::endl;
	updateArrowDirection(key, -1);
}

