#include "sky.hpp"
#include "vector3.hpp"
#include "vertex.hpp"
#include <cmath>
#include <utility>
#include <map>
#include <iostream>
#include <boost/foreach.hpp>

const float PI = 3.14159265358979;
const unsigned int num_edges = 30;

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

Vertex sphereVertex(unsigned int i, unsigned int j, float radius)
{
	float real_i = i / static_cast<float>(num_edges) * 360.0;
	float real_j = j / static_cast<float>(num_edges) * 360.0 / 2;

	Vertex vertex;
	vertex.position = sphereVector3(real_i, real_j) * radius;
	vertex.color = Color(
		std::sin(real_j / 180.0 * PI) / 2.0,
		std::sin(real_j / 180.0 * PI) / 2.0,
		1.0
	);

	return vertex;
}

typedef std::pair<unsigned int, unsigned int> IJPair;
typedef std::map<IJPair, unsigned int> VertexIndexMap;

unsigned int getVertexIndex(VertexIndexMap& vertex_indexes, std::vector<Vertex>& vertexes, unsigned int i, unsigned int j, float radius)
{
	IJPair ij(i, j);
	
	VertexIndexMap::const_iterator it = vertex_indexes.find(ij);
	if(it != vertex_indexes.end())
		return it->second;
	else
	{
		vertexes.push_back(sphereVertex(i, j, radius));
		vertex_indexes[ij] = vertexes.size() - 1;
		return vertexes.size() - 1;
	}
}

void Sky::buildVBO()
{
	_vbo.reset(new VertexBufferObject(VertexBufferObject::ArrayBuffer));
	_ibo.reset(new VertexBufferObject(VertexBufferObject::ElementArrayBuffer));

	std::vector<Vertex> vertexes;
	std::vector<unsigned short> indexes;

	VertexIndexMap vertex_indexes;

	// Dividing j by two to make it a dome. Without that, it would be a sphere.
	for(unsigned int j = 0; j < num_edges / 2; j++)
	{
		bool first_j = (j == 0);
		bool last_j  = (j + 1 == num_edges);

		for(unsigned int i = 0; i <= num_edges; i++)
		{
			// The first "row" has only a height (z) no width (x, y), so just do 0 there.
			indexes.push_back(getVertexIndex(vertex_indexes, vertexes, first_j ? 0 : i, j, _radius));
			// The last "row" also has only a height, so we do i = 0 here as well.
			indexes.push_back(getVertexIndex(vertex_indexes, vertexes, last_j  ? 0 : i, j + 1, _radius));
		}
	}

	std::cout << "Number of vertexes: " << vertexes.size() << std::endl;
	std::cout << "Number of indexes: " << indexes.size() << std::endl;

	_vbo->fill(vertexes, VertexBufferObject::StaticDraw);
	_ibo->fill(indexes, VertexBufferObject::StaticDraw);
}

void Sky::draw()
{
	if(_vbo.get() == 0)
		buildVBO();

	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);

	_vbo->bind();
	_ibo->bind();


	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	unsigned int vertex_size = sizeof(Vertex);
	glVertexPointer(3, GL_FLOAT, vertex_size, (void*)0);
	//glNormalPointer(GL_FLOAT, vertex_size, (void*)12);
	glColorPointer(3, GL_FLOAT, vertex_size, (void*)24);

	glPointSize(1.0);
	glDrawElements(GL_TRIANGLE_STRIP, _ibo->size(), GL_UNSIGNED_SHORT, 0);

	glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_FOG);
	glEnable(GL_LIGHTING);
}

