#ifndef PATH_HPP
#define PATH_HPP

#include <vector>
#include "vector3.hpp"

class Path
{
	public:
		void addPoint(float x, float y, float z)
		{
			_points.push_back(Vector3(x, y, z));
		}

		void draw();
		float length();
		Vector3 positionAt(float length);

	private:
		std::vector<Vector3> _points;
};

#endif

