#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "color.hpp"
#include "vertex_buffer_object.hpp"
#include "vector3.hpp"

class Heightmap
{
	public:
		Heightmap(unsigned int width, unsigned int height);

		static Heightmap loadFromFile(std::string filename);

		float heightAt(unsigned int x, unsigned int y) const
		{
			return _heights.at(posToIndex(x, y));
		}

		void setHeightAt(unsigned int x, unsigned int y, float z)
		{
			_heights[posToIndex(x, y)] = z;
		}

		Vector3 positionAt(unsigned int x, unsigned int y) const
		{
			return Vector3(x, y, heightAt(x, y));
		}

		Color& colorAt(unsigned int x, unsigned int y)
		{
			return _colors.at(posToIndex(x, y));
		}

		void update();
		void draw();

	protected:
		const unsigned int _width;
		const unsigned int _height;
		std::vector<float> _heights;
		std::vector<Color> _colors;

		void setupVBO();
		void setupNormalsVBO();
		void drawNormals();

		Vector3 vertexNormalAt(unsigned int x, unsigned int y);

		unsigned int posToIndex(unsigned int x, unsigned int y) const
		{
			return y * _width + x;
		}

		Vector3 surfaceNormal(unsigned int x, unsigned int y);

		std::vector<Vector3> _normals;

		boost::shared_ptr<VertexBufferObject> _vbo;
		boost::shared_ptr<VertexBufferObject> _ibo;
		boost::shared_ptr<VertexBufferObject> _normals_vbo;
		bool _vertex_buffers_filled; // temporary, should be removed when data can be updated after update()
};

#endif

