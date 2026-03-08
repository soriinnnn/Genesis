#ifndef GENESIS_MAT4_H
#define GENESIS_MAT4_H
#include <math/Vec3.h>

namespace genesis
{
	class Mat4
	{
	public:
		Mat4();

		float determinant() const noexcept;
		Mat4 inverse() const noexcept;
		Mat4 transpose() const noexcept;

		const float* operator[](int i) const noexcept;
		float* operator[](int i) noexcept;

		Mat4 operator+(const Mat4& rhs) const noexcept;
		Mat4 operator-(const Mat4& rhs) const noexcept;
		Mat4 operator*(const Mat4& rhs) const noexcept;
		Mat4& operator+=(const Mat4& rhs) noexcept;
		Mat4& operator-=(const Mat4& rhs) noexcept;
		Mat4& operator*=(const Mat4& rhs) noexcept;
		
		static Mat4 identity() noexcept;
		static Mat4 fromTranslation(const Vec3& translation) noexcept;
		static Mat4 fromScale(const Vec3& scale) noexcept;
		static Mat4 fromRotation(const Vec3& rotation) noexcept;
		static Mat4 fromRotationX(float rotX) noexcept;
		static Mat4 fromRotationY(float rotY) noexcept;
		static Mat4 fromRotationZ(float rotZ) noexcept;

		static Mat4 orthographicLH(float width, float height, float znear, float zfar) noexcept;
		static Mat4 orthographicOffCenterLH(float left, float right, float bottom, float top, float znear, float zfar) noexcept;
		static Mat4 perspectiveLH(float fov, float aspect, float znear, float zfar) noexcept;
		static Mat4 lookAtLH(const Vec3& eye, const Vec3& target, const Vec3& up) noexcept;

	private:
		float m_data[4][4];
	};
}

#endif