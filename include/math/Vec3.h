#ifndef GENESIS_VEC3_H
#define GENESIS_VEC3_H
#include <core/Core.h>

GENESIS_NAMESPACE_BEGIN

class Vec3
{
public:
	Vec3();
	Vec3(float x, float y, float z);

	const float* toArray() const noexcept;
	float dot(const Vec3& v) const noexcept;
	Vec3 cross(const Vec3& v) const noexcept;
	Vec3 normalize() const;
	float norm() const;

	Vec3 operator-() const noexcept;
	Vec3 operator+(const Vec3& rhs) const noexcept;
	Vec3 operator-(const Vec3& rhs) const noexcept;
	Vec3 operator*(const Vec3& rhs) const noexcept;
	Vec3 operator*(float scalar) const noexcept;
	Vec3 operator/(float scalar) const noexcept;
	Vec3& operator+=(const Vec3& rhs) noexcept;
	Vec3& operator-=(const Vec3& rhs) noexcept;
	Vec3& operator*=(const Vec3& rhs) noexcept;
	Vec3& operator*=(float scalar) noexcept;
	Vec3& operator/=(float scalar) noexcept;

	static float norm(const Vec3& v);
	static Vec3 normalize(const Vec3& v);
	static float dot(const Vec3& a, const Vec3& b) noexcept;
	static Vec3 cross(const Vec3& a, const Vec3& b) noexcept;
	static Vec3 lerp(const Vec3& a, const Vec3& b, float t) noexcept;

public:
	float x, y, z;
};

GENESIS_NAMESPACE_END

#endif