#include <math/Mat4.h>
#include <cassert>
#include <cmath>

using namespace genesis;
using namespace std;

Mat4::Mat4(): m_data({}) {}

Mat4 Mat4::transpose() const noexcept
{
	Mat4 res{};

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			res.m_data[col][row] = m_data[row][col];
		}
	}

	return res;
}

Mat4 Mat4::operator*(const Mat4& rhs) const noexcept
{
	Mat4 res{};

	for (int row = 0; row < 4; row++) {
		for (int k = 0; k < 4; k++) {
			float temp = m_data[row][k];

			for (int col = 0; col < 4; col++) {
				res.m_data[row][col] += temp * rhs.m_data[k][col];
			}
		}
	}

	return res;
}

Mat4 Mat4::identity() noexcept
{
	Mat4 res{};

	res.m_data[0][0] = 1;
	res.m_data[1][1] = 1;
	res.m_data[2][2] = 1;
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::orthographicLH(float width, float height, float near, float far) noexcept
{
	Mat4 res{};

	res.m_data[0][0] = 2.0f / width;
	res.m_data[1][1] = 2.0f / height;
	res.m_data[2][2] = 1.0f / (far - near);
	res.m_data[3][2] = -(near / (far - near));
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::orthographicOffCenterLH(float left, float right, float bottom, float top, float near, float far) noexcept
{
	Mat4 res{};

	res.m_data[0][0] = 2.0f / (right - left);
	res.m_data[1][1] = 2.0f / (top - bottom);
	res.m_data[2][2] = 1.0f / (far - near);
	res.m_data[3][0] = -(right + left) / (right - left);
	res.m_data[3][1] = -(top + bottom) / (top - bottom);
	res.m_data[3][2] = -(near / (far - near));
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::fromTranslation(const Vec3& translation) noexcept
{
	Mat4 res = identity();

	res.m_data[3][0] = translation.x;
	res.m_data[3][1] = translation.y;
	res.m_data[3][2] = translation.z;

	return res;
}

Mat4 Mat4::fromScale(const Vec3& scale) noexcept
{
	Mat4 res{};

	res.m_data[0][0] = scale.x;
	res.m_data[1][1] = scale.y;
	res.m_data[2][2] = scale.z;
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::fromRotation(const Vec3& rotation) noexcept
{
	Mat4 rotX = Mat4::fromRotationX(rotation.x);
	Mat4 rotY = Mat4::fromRotationY(rotation.y);
	Mat4 rotZ = Mat4::fromRotationZ(rotation.z);

	return rotX * rotY * rotZ;
}

Mat4 Mat4::fromRotationX(float rotX) noexcept
{
	Mat4 res{};
	float sinX = sin(rotX);
	float cosX = cos(rotX);

	res.m_data[0][0] = 1;
	res.m_data[1][1] = cosX;
	res.m_data[1][2] = sinX;
	res.m_data[2][1] = -sinX;
	res.m_data[2][2] = cosX;
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::fromRotationY(float rotY) noexcept
{
	Mat4 res{};
	float sinY = sin(rotY);
	float cosY = cos(rotY);

	res.m_data[0][0] = cosY;
	res.m_data[0][2] = -sinY;
	res.m_data[1][1] = 1;
	res.m_data[2][0] = sinY;
	res.m_data[2][2] = cosY;
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::fromRotationZ(float rotZ) noexcept
{
	Mat4 res{};
	float sinZ = sin(rotZ);
	float cosZ = cos(rotZ);

	res.m_data[0][0] = cosZ;
	res.m_data[0][1] = sinZ;
	res.m_data[1][0] = -sinZ;
	res.m_data[1][1] = cosZ;
	res.m_data[2][2] = 1;
	res.m_data[3][3] = 1;

	return res;
}