#ifndef GENESIS_MAT4_H
#define GENESIS_MAT4_H
#include <math/Vec3.h>

namespace genesis
{
	class Mat4
	{
	public:
		Mat4();

		Mat4 operator *(const Mat4& rhs) const noexcept;

		static Mat4 identity() noexcept;
		static Mat4 fromTranslation(const Vec3& translation) noexcept;
		static Mat4 fromScale(const Vec3& scale) noexcept;
		static Mat4 fromRotation(const Vec3& rotation) noexcept;
		static Mat4 fromRotationX(float rotX) noexcept;
		static Mat4 fromRotationY(float rotY) noexcept;
		static Mat4 fromRotationZ(float rotZ) noexcept;

	private:
		float m_data[4][4];
	};
}

#endif