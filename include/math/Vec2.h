#ifndef GENESIS_VEC2_H
#define GENESIS_VEC2_H

namespace genesis
{
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

		static float dot(const Vec2& v1, const Vec2& v2) noexcept;
		static float cross(const Vec2& v1, const Vec2& v2) noexcept;
		static Vec2 normalize(const Vec2& v) noexcept;

	public:
		float x, y;
	};
}

#endif