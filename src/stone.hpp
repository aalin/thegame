#ifndef STONE_HPP
#define STONE_HPP

#include <chipmunk.h>

class Stone
{
	public:
		Stone(cpSpace* space, float x, float radius);
		~Stone();

		float x() const { return _body->p.x; }
		float y() const { return _body->p.y; }
		float radius() const { return cpCircleShapeGetRadius(_shape); }
		float angle() const { return _body->p.y; }

	private:
		cpBody* _body;
		cpShape* _shape;
};

#endif

