#ifndef GENESIS_VEC2_H
#define GENESIS_VEC2_H
#include <core/Core.h>

GENESIS_NAMESPACE_START

class Vec2
{
public:
	Vec2();
	Vec2(float x, float y);

	const float* toArray() const noexcept;
	float dot(const Vec2& v) const noexcept;
	float cross(const Vec2& v) const noexcept;
	Vec2 normalize() const noexcept;

	Vec2 operator+(const Vec2& rhs) const noexcept;
	Vec2 operator-(const Vec2& rhs) const noexcept;
	Vec2 operator*(float scalar) const noexcept;
	Vec2 operator/(float scalar) const noexcept;
	Vec2& operator+=(const Vec2& rhs) noexcept;
	Vec2& operator-=(const Vec2& rhs) noexcept;
	Vec2& operator*=(float scalar) noexcept;
	Vec2& operator/=(float scalar) noexcept;

	bool operator==(const Vec2& rhs) const noexcept;

	static float dot(const Vec2& v1, const Vec2& v2) noexcept;
	static float cross(const Vec2& v1, const Vec2& v2) noexcept;
	static Vec2 normalize(const Vec2& v) noexcept;

public:
	float x, y;
};

GENESIS_NAMESPACE_END

#endif