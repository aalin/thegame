#include "heightmap.hpp"
#include "triangle.hpp"
#include "opengl.hpp"
#include "vertex.hpp"
#include "util.hpp"
#include <iostream>
#include <cmath>

Heightmap::Heightmap(unsigned int width, unsigned int height)
	: _width(width), _height(height)
{

	_heights.resize(_width * _height);
	_colors.resize(_width * _height);
	for(size_t y = 0; y < _height; y++)
	{
		for(size_t x = 0; x < _width; x++)
		{
			colorAt(x, y) = Color(
				x * 1.0 / _width,
				y * 1.0 / _height,
				(x + y) * 1.0 / (_width + _height)
			);
		}
	}
	_vertex_buffers_filled = false;
}

Heightmap
Heightmap::loadFromFile(std::string filename)
{
	std::vector<char> data(Util::loadFile(filename.c_str()));

	unsigned int heightmap_size = std::sqrt(data.size() / 4);

	Heightmap heightmap(heightmap_size, heightmap_size);

	for(size_t y = 0; y < heightmap_size - 1; y++)
	{
		for(size_t x = 0; x < heightmap_size - 1; x++)
		{
			size_t index = y * heightmap_size * 4 + x * 4 + 4;
			unsigned char value = data.at(index);
			float height = value / 255.0 * 64.0;
			heightmap.setHeightAt(x, y, height);
		}
	}
	
	return heightmap;
}

void Heightmap::drawVertex(unsigned int x, unsigned int y)
{
	colorAt(x, y).draw();
	Vector3 normal(vertexNormalAt(x, y));
	glNormal3f(normal.x, normal.y, normal.z);
	Vector3 pos(positionAt(x, y));
	glVertex3f(pos.x, pos.y, pos.z);
}

Vector3 Heightmap::surfaceNormal(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	if(x0 >= _width || x1 >= _width || x2 >= _width)
		return Vector3(0.0, 0.0, 0.0);
	if(y0 >= _height || y1 >= _height || y2 >= _height)
		return Vector3(0.0, 0.0, 0.0);

	Vector3 v0 = positionAt(x0, y0);
	Vector3 v1 = positionAt(x1, y1);
	Vector3 v2 = positionAt(x2, y2);

	Vector3 n0 = Triangle(v0, v2, v1).getNormal();
	Vector3 n1 = Triangle(v2, v0, v1).getNormal();
	Vector3 n2 = Triangle(v1, v0, v2).getNormal();

	return (n0 + n1 + n2).normalize();
}

Vector3 Heightmap::vertexNormalAt(unsigned int x, unsigned int y)
{
	return (
		surfaceNormal(x, y, x, y+1, x+1, y+1)
	).normalize();

	Vector3 a(0, 0, 0);
	a += surfaceNormal(x, y, x-1, y, x-1, y+1);
	a += surfaceNormal(x, y, x-1, y+1, x, y+1);
	a += surfaceNormal(x, y, x, y+1, x+1, y);
	a += surfaceNormal(x, y, x+1, y, x+1, y-1);
	a += surfaceNormal(x, y, x+1, y-1, x, y-1);
	a += surfaceNormal(x, y, x, y-1, x-1, y);

	return a.normalize();
}

void Heightmap::update()
{
	if(_vertex_buffers_filled)
		return;
	_vertex_buffers_filled = true;
	_vbo.reset(new VertexBufferObject(VertexBufferObject::ArrayBuffer));
	_ibo.reset(new VertexBufferObject(VertexBufferObject::ElementArrayBuffer));

	std::vector<Vertex> vertex_data;
	for(size_t x = 0; x < _width; x++)
	{
		for(size_t y = 0; y < _height; y++)
		{
			Vertex vertex;
			vertex.position = positionAt(x, y);
			vertex.normal   = vertexNormalAt(x, y);
			vertex.color    = colorAt(x, y);
			vertex_data.push_back(vertex);
		}
	}

	_vbo->fill(vertex_data, VertexBufferObject::StaticDraw);

	std::vector<unsigned short> indexes;
	for(size_t x = 0; x < _width ; x++)
	{
		bool even = x % 2 == 0;
		if(even)
			for(size_t y = 0; y < _height ; y++)
				addIndex(indexes, x, y);
		else
			for(size_t y = _height - 1; y; y--)
				addIndex(indexes, x, y);
	}

	_ibo->fill(indexes, VertexBufferObject::StaticDraw);
}

void Heightmap::draw()
{
	_vbo->bind();
	_ibo->bind();
	glEnableClientState(GL_VERTEX_ARRAY);
	unsigned int vertex_size = sizeof(Vertex);
	glVertexPointer(3, GL_FLOAT, vertex_size, (void*)0);
	glNormalPointer(GL_FLOAT, vertex_size, (void*)12);
	glColorPointer(3, GL_FLOAT, vertex_size, (void*)24);
	glDrawElements(GL_TRIANGLE_STRIP, (_width - 1) * (_height - 1) * 2, GL_UNSIGNED_SHORT, 0);
	glDisableClientState(GL_VERTEX_ARRAY);

	return;
	// Debug normals
	glDisable(GL_LIGHTING);
	for(size_t y = 0; y < _height - 1; y++)
	{
		for(size_t x = 0; x < _width - 1; x++)
		{
			Vector3 p1(positionAt(x, y));
			Vector3 p2(p1 + vertexNormalAt(x, y));

			glBegin(GL_LINES);
			glColor3f(0.7, 1.0, 0.7);
			glVertex3f(p1.x, p1.y, p1.z);
			glColor3f(0.7, 0.7, 1.0);
			glVertex3f(p2.x, p2.y, p2.z);
			glEnd();
		}
	}
	glEnable(GL_LIGHTING);
}

