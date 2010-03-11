#include "cube_scene.hpp"
#include <SDL/SDL.h>
#include <cmath>
#include <iostream>
#include <boost/foreach.hpp>

CubeScene::CubeScene()
	: _sky(256),
	  _heightmap(Heightmap::loadFromFile("data/asd"))
{
	_path.addPoint(97, 30, 0.0);
	_path.addPoint(93, 17, 0.0);
	_path.addPoint(85, 12, 0.0);
	_path.addPoint(71, 24, 0.0);
	_path.addPoint(68, 42, 0.0);
	_path.addPoint(89, 67, 0.0);
	_path.addPoint(83, 78, 0.0);
	_path.addPoint(75, 138, 0.0);
	_path.addPoint(91, 169, 0.0);
	_path.addPoint(67, 207, 0.0);
	_path.addPoint(69, 229, 0.0);
	_path.addPoint(63, 241, 0.0);
	_path.addPoint(93, 248, 0.0);
	_path.addPoint(108, 231, 0.0);
	_path.addPoint(138, 237, 0.0);
	_path.addPoint(143, 220, 0.0);
	_path.addPoint(154, 211, 0.0);
	_path.addPoint(157, 204, 0.0);
	_path.addPoint(169, 200, 0.0);
	_path.addPoint(170, 192, 0.0);
	_path.addPoint(158, 187, 0.0);
	_path.addPoint(151, 164, 0.0);
	_path.addPoint(174, 152, 0.0);
	_path.addPoint(193, 166, 0.0);
	_path.addPoint(215, 217, 0.0);
	_path.addPoint(226, 220, 0.0);
	_path.addPoint(232, 205, 0.0);
	_path.addPoint(227, 179, 0.0);
	_path.addPoint(243, 164, 0.0);
	_path.addPoint(245, 154, 0.0);
	_path.addPoint(231, 145, 0.0);
	_path.addPoint(230, 135, 0.0);
	_path.addPoint(241, 125, 0.0);
	_path.addPoint(239, 108, 0.0);
	_path.addPoint(213, 87, 0.0);
	_path.addPoint(213, 79, 0.0);
	_path.addPoint(197, 64, 0.0);
	_path.addPoint(214, 51, 0.0);
	_path.addPoint(217, 20, 0.0);
	_path.addPoint(210, 16, 0.0);
	_path.addPoint(177, 44, 0.0);
	_path.addPoint(163, 48, 0.0);
	_path.addPoint(155, 56, 0.0);
	_path.addPoint(161, 74, 0.0);
	_path.addPoint(180, 99, 0.0);
	_path.addPoint(181, 111, 0.0);
	_path.addPoint(171, 120, 0.0);

	_path.smoothen();
	_path.setHeightsFromHeightmap(_heightmap);

	setupSpace();

	for(unsigned int i = 0; i < 10; i++)
		_stones.push_back(boost::shared_ptr<Stone>(new Stone(_space, i / (20.0 + 10) * _path.length())));

	_arrow_direction = cpv(0, 0);
	_last_jump_state = false;
}

void playerUpdateVelocity(cpBody *body, cpVect gravity, cpFloat damping, cpFloat dt)
{
	cpBodyUpdateVelocity(body, gravity, damping, dt);
	body->v.x = cpfclamp(body->v.x, -10, 10);
	body->v.y = cpfmax(body->v.y, -10);
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
	_space->gravity = cpv(0, -10);
	
	_static_body = cpBodyNew(INFINITY, INFINITY);

	for(unsigned int i = 0; i < _path.length() - 1; i++)
	{
		Vector3 pos1(_path.positionAt(i));
		Vector3 pos2(_path.positionAt(i + 1));
		addGroundShape(_space, _static_body, cpv(i, pos1.z), cpv(i + 1, pos2.z));
	}

	// Player

	//_player_body = cpSpaceAddBody(_space, cpBodyNew(10.0, INFINITY));
	_player_body = cpSpaceAddBody(_space, cpBodyNew(80.0, 1.0));
	_player_body->p = cpv(20.0, _path.positionAt(20.0).z + 1);
	_player_body->velocity_func = &playerUpdateVelocity;

	_player_shape = cpSpaceAddShape(_space, cpCircleShapeNew(_player_body, 1.0, cpvzero));
	_player_shape->e = 0.1;
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
	cpBodyApplyImpulse(_player_body, cpv(_arrow_direction.x * 5.0, 0.0), cpvzero);

	bool jump_state = _arrow_direction.y > 0;

	if(jump_state && !_last_jump_state && cpvlengthsq(_player_ground_normal))
	{
		cpBodyApplyImpulse(_player_body, cpv(0.0, 300.0), cpv(0.0, 0.0));
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

	for(int i = 0; i < 10; i++)
	{
		float offset = _player_body->p.x + i * 4.0;
		if(offset < 0.0) continue; // offset = 0.0;
		if(offset > _path.length()) break;
		Vector3 offset_pos = _path.positionAt(offset) * Vector3(1.0, 1.0, 0.0) + Vector3(0.0, 0.0, _player_body->p.y);
		normal += (offset_pos - position).normalize() * std::cos(i / 20.0 * M_PI);
	}

	normal.normalize();

	Vector3 camera_pos(
		position.x + normal.x * -10,
		position.y + normal.y * -10,
		0.0
	);

	float px = (camera_pos.x < 1.0) ? 1.0 : (camera_pos.x > _heightmap.width() - 2 ? _heightmap.width() - 2: camera_pos.x);
	float py = (camera_pos.y < 1.0) ? 1.0 : (camera_pos.y > _heightmap.height() - 2 ? _heightmap.height() - 2: camera_pos.y);
	float height_at_pos = _heightmap.interpolatedHeightAt(px, py);
	camera_pos.z = height_at_pos + 5.0;

	gluLookAt(
		camera_pos.x, camera_pos.y, camera_pos.z,
		position.x, position.y, position.z,
		0.0, 0.0, 1.0
	);
}

void drawCircle(float radius, bool draw_from_center)
{
	glBegin(GL_LINE_STRIP);
	if(draw_from_center)
		glVertex3f(0.0, 0.0, 0.0);
	const int detail = 8;
	for(int i = 0; i <= detail; i++)
	{
		glVertex3f(
			std::cos(i * 360 / detail / 180.0 * M_PI) * radius,
			0.0,
			std::sin(i * 360.0 / detail / 180.0 * M_PI) * radius
		);
	}
	glEnd();
}

void CubeScene::drawPlayer()
{
	Vector3 pos(_path.positionAt(_player_body->p.x));
	Vector3 normal;
	for(int x = 0; x < 5; x++)
		normal += (_path.positionAt(_player_body->p.x + x) - pos).normalize() * (x / 5.0);
	normal.normalize();
	normal *= 180.0;

	pos.z = _player_body->p.y;

	glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(0.0, 0.0, 0.0 - _player_body->a, 1.0);
		drawCircle(1.0, true);
	glPopMatrix();
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

	BOOST_FOREACH(boost::shared_ptr<Stone> stoneptr, _stones)
	{
		Vector3 pos = _path.positionAt(stoneptr->x());
		glPushMatrix();
			glTranslatef(pos.x, pos.y, stoneptr->y());
			drawCircle(stoneptr->radius(), false);
		glPopMatrix();
	}
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

