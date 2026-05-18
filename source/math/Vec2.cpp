#include <math/Vec2.h>
#include <cmath>

using namespace genesis;
using namespace std;

Vec2::Vec2()
{
	this->x = 0;
	this->y = 0;
}

Vec2::Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

const float* Vec2::toArray() const noexcept
{
	return &this->x;
}

float Vec2::dot(const Vec2& v) const noexcept
{
	return this->x * v.x + this->y * v.y;
}

float Vec2::cross(const Vec2& v) const noexcept
{
	return this->x * v.y - this->y * v.x;
}

Vec2 Vec2::normalize() const noexcept
{
	float distSq = x * x + y * y;

	if (distSq < 1e-8f) {
		return *this;
	}

	float dist = sqrt(distSq);
	float invDist = 1.0f / dist;

	return Vec2
	{
		x * invDist,
		y * invDist
	};
}

Vec2 Vec2::operator+(const Vec2& rhs) const noexcept
{
	return Vec2
	{
		this->x + rhs.x,
		this->y + rhs.y
	};
}

Vec2 Vec2::operator-(const Vec2& rhs) const noexcept
{
	return Vec2
	{
		this->x - rhs.x,
		this->y - rhs.y
	};
}

Vec2 Vec2::operator*(float scalar) const noexcept
{
	return Vec2
	{
		this->x * scalar,
		this->y * scalar
	};
}

Vec2 Vec2::operator/(float scalar) const noexcept
{
	float invScalar = 1.0f / scalar;
	return Vec2
	{
		this->x * invScalar,
		this->y * invScalar
	};
}

Vec2& Vec2::operator+=(const Vec2& rhs) noexcept
{
	this->x += rhs.x;
	this->y += rhs.y;
	
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs) noexcept
{
	this->x -= rhs.x;
	this->y -= rhs.y;

	return *this;
}

Vec2& Vec2::operator*=(float scalar) noexcept
{
	this->x *= scalar;
	this->y *= scalar;

	return *this;
}

Vec2& Vec2::operator/=(float scalar) noexcept
{
	float invScalar = 1.0f / scalar;

	this->x *= invScalar;
	this->y *= invScalar;

	return *this;
}

bool Vec2::operator==(const Vec2& rhs) const noexcept
{
	return this->x == rhs.x && this->y == rhs.y;
}

float Vec2::dot(const Vec2& v1, const Vec2& v2) noexcept
{
	return v1.dot(v2);
}

float Vec2::cross(const Vec2& v1, const Vec2& v2) noexcept
{
	return v1.cross(v2);
}

Vec2 Vec2::normalize(const Vec2& v) noexcept
{
	return v.normalize();
}