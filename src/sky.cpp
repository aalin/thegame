#include "sky.hpp"
#include "vector3.hpp"
#include <cmath>

const float PI = 3.14159265358979;

Sky::Sky(float radius)
{
	_radius = radius;
}

Vector3 sphereVector3(unsigned int i, unsigned int j)
{
	return Vector3(
		std::cos(i / 180.0 * PI) * std::cos(j / 180.0 * PI),
		std::sin(i / 180.0 * PI) * std::cos(j / 180.0 * PI),
		std::sin(j / 180.0 * PI)
	);
}

void Sky::draw()
{
	glDisable(GL_FOG);

	glColor3f(0.5, 0.5, 1.0);

	unsigned int detail = 4;

	for(unsigned int i = 0; i <= 180; i += detail)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(unsigned int j = 0; j <= 360; j += detail)
		{
			Vector3 v1 = sphereVector3(i, j) * _radius;
			Vector3 v2 = sphereVector3(i + detail, j) * _radius;
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
		}
		glEnd();
	}

	glEnable(GL_FOG);
}

