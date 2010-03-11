#ifndef STONE_HPP
#define STONE_HPP

#include <chipmunk.h>

class Stone
{
	public:
		Stone(cpSpace* space, float x);
		~Stone();

		float x() const { return _body->p.x; }
		float y() const { return _body->p.y; }
		float radius() const { return cpCircleShapeGetRadius(_shape); }

	private:
		cpBody* _body;
		cpShape* _shape;
};

#endif

