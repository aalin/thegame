#include "stone.hpp"

Stone::Stone(cpSpace* space, float x)
{
	_body = cpSpaceAddBody(space, cpBodyNew(100.0, 200.0));
	_body->p = cpv(x, 100);
	_shape = cpSpaceAddShape(space, cpCircleShapeNew(_body, 0.2, cpvzero));
	_shape->e = 0.1;
	_shape->u = 0.8;
}

Stone::~Stone()
{
}

