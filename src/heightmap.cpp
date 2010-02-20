#include "heightmap.hpp"
#include "triangle.hpp"
#include "opengl.hpp"
#include "vertex.hpp"
#include "util.hpp"
#include "image.hpp"
#include <iostream>
#include <cmath>

Heightmap::Heightmap(unsigned int width, unsigned int height)
	: _width(width), _height(height)
{
	_heights.resize(_width * _height);
	_colors.resize(_width * _height);
	_vertex_buffers_filled = false;
}

Heightmap
Heightmap::loadFromFile(std::string filename)
{
	Image image(filename);

	Heightmap heightmap(image.getWidth(), image.getHeight());

	for(size_t y = 0; y < image.getHeight() - 1; y++)
	{
		for(size_t x = 0; x < image.getWidth() - 1; x++)
		{
			float height = image.at(x, y).r * 40;
			heightmap.setHeightAt(x, y, height);
			heightmap.colorAt(x, y) = Color(std::sin(height / 40.0), std::cos(height / 40.0), std::sin(height / 80.0));
		}
	}
	
	return heightmap;
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
	return surfaceNormal(x, y, x+1, y, x +1, y-1);
}

void Heightmap::setupVBO()
{
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
		for(size_t y = 0; y < _height ; y++)
		{
			indexes.push_back(posToIndex(x, y));
			indexes.push_back(posToIndex(x+1, y));
		}
	}

	_ibo->fill(indexes, VertexBufferObject::StaticDraw);
}

void Heightmap::update()
{
	if(_vbo.get() == 0)
		setupVBO();
}

void Heightmap::setupNormalsVBO()
{
	_normals_vbo.reset(new VertexBufferObject(VertexBufferObject::ArrayBuffer));
	std::vector<Vector3> vectors;

	for(unsigned int x = 0; x < _width; x++)
	{
		for(unsigned int y = 0; y < _height; y++)
		{
			Vector3 pos(positionAt(x, y));
			vectors.push_back(pos);
			vectors.push_back(pos + vertexNormalAt(x, y));
		}
	}

	_normals_vbo->fill(vectors, VertexBufferObject::StaticDraw);
}

void Heightmap::drawNormals()
{
	if(_normals_vbo.get() == 0)
		setupNormalsVBO();

	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);

	_normals_vbo->bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vector3), (void*)0);
	glDrawArrays(GL_LINES, 0, _normals_vbo->size());
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);
}

void Heightmap::draw()
{
	_vbo->bind();
	_ibo->bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	unsigned int vertex_size = sizeof(Vertex);
	glVertexPointer(3, GL_FLOAT, vertex_size, (void*)0);
	glNormalPointer(GL_FLOAT, vertex_size, (void*)12);
	glColorPointer(3, GL_FLOAT, vertex_size, (void*)24);

	for(unsigned int x = 0; x < _width - 2; x++)
		glDrawElements(GL_TRIANGLE_STRIP, _height * 2, GL_UNSIGNED_SHORT, (void*)(x * 2 * _height * 2));

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	drawNormals();
}

