#ifndef GENESIS_VEC3_H
#define GENESIS_VEC3_H

namespace genesis
{
	class Vec3
	{
	public:
		Vec3();
		Vec3(float x, float y, float z);

		const float* toArray() const noexcept;
		float dot(const Vec3& v) const noexcept;
		Vec3 cross(const Vec3& v) const noexcept;
		Vec3 normalize() const noexcept;

		Vec3 operator+(const Vec3& rhs) const noexcept;
		Vec3 operator-(const Vec3& rhs) const noexcept;
		Vec3 operator*(float scalar) const noexcept;
		Vec3 operator/(float scalar) const noexcept;
		Vec3& operator+=(const Vec3& rhs) noexcept;
		Vec3& operator-=(const Vec3& rhs) noexcept;
		Vec3& operator*=(float scalar) noexcept;
		Vec3& operator/=(float scalar) noexcept;

		static float dot(const Vec3& v1, const Vec3& v2) noexcept;
		static Vec3 cross(const Vec3& v1, const Vec3& v2) noexcept;
		static Vec3 normalize(const Vec3& v) noexcept;

	public:
		float x, y, z;
	};
}

#endif