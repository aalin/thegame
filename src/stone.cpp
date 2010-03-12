#include "stone.hpp"

Stone::Stone(cpSpace* space, float x, float radius)
{
	_body = cpSpaceAddBody(space, cpBodyNew(5.0, 0.5));
	_body->p = cpv(x, 100);
	_shape = cpSpaceAddShape(space, cpCircleShapeNew(_body, radius, cpvzero));
	_shape->e = 0.0;
	_shape->u = 2.0;
}

Stone::~Stone()
{
}

