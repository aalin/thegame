#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <ostream>

struct Vector3
{
	Vector3() : x(0.0), y(0.0), z(0.0)
	{ }

	Vector3(float x, float y, float z) : x(x), y(y), z(z)
	{ }

	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z)
	{ }

	Vector3& operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3& operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3& operator*=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Vector3 operator-(const Vector3& v) const { return Vector3(*this) -= v; }
	Vector3 operator+(const Vector3& v) const { return Vector3(*this) += v; }
	Vector3 operator*(float f) const { return Vector3(*this) *= f; }
		
	bool operator==(const Vector3& v) const;

	Vector3 crossProduct(const Vector3& v) const;
	float dotProduct(const Vector3& v) const;
	float getMagnitude() const;

	Vector3& normalize();
	Vector3 getNormalized() const;

	friend std::ostream& operator<<(std::ostream& o, const Vector3& v);

	float x, y, z;
};

#endif

