#ifndef SKY_HPP
#define SKY_HPP

#include "vertex_buffer_object.hpp"

class Sky
{
	public:
		Sky(float radius);
		void draw();

	private:
		float _radius;
};

#endif

