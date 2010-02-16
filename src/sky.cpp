#include "sky.hpp"
#include "vector3.hpp"
#include "vertex.hpp"
#include <cmath>

#include <iostream>

const float PI = 3.14159265358979;

Sky::Sky(float radius)
{
	_radius = radius;
}

Vector3 sphereVector3(unsigned int i, unsigned int j)
{
	return Vector3(
		std::cos(i / 180.0 * PI) * std::sin(j / 180.0 * PI),
		std::sin(i / 180.0 * PI) * std::sin(j / 180.0 * PI),
		std::cos(j / 180.0 * PI)
	);
}

void Sky::buildVBO()
{
	_vbo.reset(new VertexBufferObject(VertexBufferObject::ArrayBuffer));
	_ibo.reset(new VertexBufferObject(VertexBufferObject::ElementArrayBuffer));

	std::vector<Vertex> vertex_data;
	std::vector<unsigned short> indexes;

	for(unsigned int j = 0; j <= 360; j += 1)
	{
		for(unsigned int i = 0; i <= 360; i += 1)
		{
			Vertex vertex;
			vertex.position = sphereVector3(i, j) * _radius;
			vertex.color = Color(std::sin(j / 180.0 * PI), 0.0, std::sin(i / 180.0 * PI));
			vertex_data.push_back(vertex);
			indexes.push_back(j * 360 + i);
			indexes.push_back((j + 1) * 360 + i);
		}
	}

	_vbo->fill(vertex_data, VertexBufferObject::StaticDraw);
	_ibo->fill(indexes, VertexBufferObject::StaticDraw);
}

void Sky::draw()
{
	if(_vbo.get() == 0)
		buildVBO();

	glDisable(GL_FOG);

	_vbo->bind();
	_ibo->bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	unsigned int vertex_size = sizeof(Vertex);
	glVertexPointer(3, GL_FLOAT, vertex_size, (void*)0);
//	glNormalPointer(GL_FLOAT, vertex_size, (void*)12);
	glColorPointer(3, GL_FLOAT, vertex_size, (void*)24);

	glPointSize(1.0);
	glDrawElements(GL_TRIANGLE_STRIP, 360 * 360, GL_UNSIGNED_SHORT, 0);

	glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_FOG);
}

