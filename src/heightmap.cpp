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

Vector3 Heightmap::surfaceNormal(unsigned int x, unsigned int y)
{
	if(x < 1 || x >= _width - 1 || y < 1 || y >= _height - 1)
		return Vector3(0, 0, 0);

	float dx = heightAt(x + 1, y) - heightAt(x - 1, y);
	float dy = heightAt(x, y + 1) - heightAt(x, y - 1);
	return Vector3(-dx, -dy, 1.0).normalize();
}

Vector3 Heightmap::vertexNormalAt(unsigned int x, unsigned int y)
{
	// Pretty smooth normals. slow though.
	const float PI = 3.14159265358979;
	const int smooth_radius = 3;

	Vector3 n;

	for(int i = -smooth_radius; i < smooth_radius; i++)
		for(int j = -smooth_radius; j < smooth_radius; j++)
			n += surfaceNormal(x + i, y + j) * std::sin(std::abs(i + j) / 2.0 / smooth_radius / 2.0 * PI);

	   return n.normalize();
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
	glColor4f(1.0, 1.0, 1.0, 0.3);

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

	// drawNormals();
}

