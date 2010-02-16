#ifndef SKY_HPP
#define SKY_HPP

#include <boost/shared_ptr.hpp>
#include "vertex_buffer_object.hpp"

class Sky
{
	public:
		Sky(float radius);
		void draw();

	private:
		float _radius;
		void buildVBO();

		boost::shared_ptr<VertexBufferObject> _vbo;
		boost::shared_ptr<VertexBufferObject> _ibo;
};

#endif

