#include "vector3.hpp"
#include <cmath>
#include <limits>

bool compareFloats(float a, float b)
{
	return std::fabs(a - b) < std::numeric_limits<float>::epsilon();
}

bool Vector3::operator==(const Vector3& v) const
{
	return compareFloats(x, v.x) && compareFloats(y, v.y) && compareFloats(z, v.z);
}

Vector3 Vector3::crossProduct(const Vector3& v) const
{
	return Vector3(
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x
	);
}

float Vector3::dotProduct(const Vector3& v) const
{
	return x * v.x +
	       y * v.y +
	       z * v.z;
}

float Vector3::getMagnitude() const
{
	return std::sqrt(std::fabs(dotProduct(*this)));
}

Vector3& Vector3::normalize()
{
	float magnitude = getMagnitude();

	if(magnitude < std::numeric_limits<float>::epsilon())
		return *this;

	x /= magnitude;
	y /= magnitude;
	z /= magnitude;

	return *this;
}

Vector3 Vector3::getNormalized() const
{
	return Vector3(*this).normalize();
}

std::ostream& operator<<(std::ostream& o, const Vector3& v)
{
	return o << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
}

