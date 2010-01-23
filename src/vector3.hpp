#ifndef VECTOR3_HPP
#define VECTOR3_HPP

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

	Vector3 operator-(const Vector3& v) const { return Vector3(*this) -= v; }
	Vector3 operator+(const Vector3& v) const { return Vector3(*this) += v; }
		

	Vector3 crossProduct(const Vector3& v) const;
	float dotProduct(const Vector3& v) const;
	float getMagnitude() const;

	Vector3& normalize();
	Vector3 getNormalized() const;

	float x, y, z;
};

#endif

