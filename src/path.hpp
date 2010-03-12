#ifndef PATH_HPP
#define PATH_HPP

#include <vector>
#include "vector3.hpp"

class Heightmap;

class Path
{
	public:
		void addPoint(float x, float y, float z)
		{
			_points.push_back(Vector3(x, y, z));
		}

		void draw() const;
		float length() const;
		Vector3 positionAt(float length) const;

		void smoothen();
		void setHeightsFromHeightmap(const Heightmap&);

		Vector3 normalAt(float position) const;
		float angleAt(float position) const;

	private:
		std::vector<Vector3> _points;
};

#endif

