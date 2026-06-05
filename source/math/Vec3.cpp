#include <math/Vec3.h>
#include <cmath>

using namespace genesis;
using namespace std;

Vec3::Vec3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

const float* Vec3::toArray() const noexcept
{
    return &this->x;
}

float Vec3::dot(const Vec3& v) const noexcept
{
	return this->x * v.x + this->y * v.y + this->z * v.z;
}

Vec3 Vec3::cross(const Vec3& v) const noexcept
{
	return Vec3
	{
		this->y * v.z - this->z * v.y,
		this->z * v.x - this->x * v.z,
		this->x * v.y - this->y * v.x
	};
}

Vec3 Vec3::normalize() const noexcept
{
	float distSq = x*x + y*y + z*z;
	
	if (distSq < 1e-8f) {
		return *this;
	}

	float dist = sqrt(distSq);
	float invDist = 1.0f / dist;

	return Vec3
	{
		x * invDist,
		y * invDist,
		z * invDist
	};
}

Vec3 Vec3::operator-() const noexcept
{
	return Vec3{-x, -y, -z};
}

Vec3 Vec3::operator+(const Vec3& rhs) const noexcept
{
	return Vec3
	{
		this->x + rhs.x, 
		this->y + rhs.y, 
		this->z + rhs.z
	};
}

Vec3 Vec3::operator-(const Vec3& rhs) const noexcept
{
	return Vec3
	{
		this->x - rhs.x,
		this->y - rhs.y,
		this->z - rhs.z
	};
}

Vec3 Vec3::operator*(float scalar) const noexcept
{
	return Vec3
	{
		this->x * scalar,
		this->y * scalar,
		this->z * scalar
	};
}

Vec3 Vec3::operator/(float scalar) const noexcept
{
	float invScalar = 1.0f / scalar;
	return Vec3
	{
		this->x * invScalar,
		this->y * invScalar,
		this->z * invScalar
	};
}

Vec3& Vec3::operator+=(const Vec3& rhs) noexcept
{
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return *this;
}

Vec3& Vec3::operator-=(const Vec3& rhs) noexcept
{
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return *this;
}

Vec3& Vec3::operator*=(float scalar) noexcept
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;

	return *this;
}

Vec3& Vec3::operator/=(float scalar) noexcept
{
	float invScalar = 1.0f / scalar;

	this->x *= invScalar;
	this->y *= invScalar;
	this->z *= invScalar;

	return *this;
}

float Vec3::dot(const Vec3& a, const Vec3& b) noexcept
{
	return a.dot(b);
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b) noexcept
{
	return a.cross(b);
}

Vec3 Vec3::normalize(const Vec3& v) noexcept
{
	return v.normalize();
}

Vec3 Vec3::lerp(const Vec3& a, const Vec3& b, float t) noexcept
{
	return a + (b - a) * t;
}