#include <math/Mat4.h>
#include <core/utils/Macros.h>
#include <cmath>

using namespace genesis;
using namespace std;

Mat4::Mat4(): m_data({}) {}

float Mat4::determinant() const noexcept
{
	float det00 = m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
	float det01 = m_data[0][0] * m_data[1][2] - m_data[0][2] * m_data[1][0];
	float det02 = m_data[0][0] * m_data[1][3] - m_data[0][3] * m_data[1][0];
	float det03 = m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1];
	float det04 = m_data[0][1] * m_data[1][3] - m_data[0][3] * m_data[1][1];
	float det05 = m_data[0][2] * m_data[1][3] - m_data[0][3] * m_data[1][2];
	float det06 = m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0];
	float det07 = m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0];
	float det08 = m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0];
	float det09 = m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1];
	float det10 = m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1];
	float det11 = m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2];

	return (det00 * det11 - det01 * det10 + det02 * det09 +
			det03 * det08 - det04 * det07 + det05 * det06);
}

Mat4 Mat4::inverse() const noexcept
{
	float det00 = m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
	float det01 = m_data[0][0] * m_data[1][2] - m_data[0][2] * m_data[1][0];
	float det02 = m_data[0][0] * m_data[1][3] - m_data[0][3] * m_data[1][0];
	float det03 = m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1];
	float det04 = m_data[0][1] * m_data[1][3] - m_data[0][3] * m_data[1][1];
	float det05 = m_data[0][2] * m_data[1][3] - m_data[0][3] * m_data[1][2];
	float det06 = m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0];
	float det07 = m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0];
	float det08 = m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0];
	float det09 = m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1];
	float det10 = m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1];
	float det11 = m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2];

	float det = det00 * det11 - det01 * det10 + det02 * det09 + det03 * det08 - det04 * det07 + det05 * det06;
	if (abs(det) < 1e-8f) {
		return Mat4::identity();
	}

	Mat4 res;
	float invDet = 1.0f / det;

	res.m_data[0][0] =  (m_data[1][1] * det11 - m_data[1][2] * det10 + m_data[1][3] * det09) * invDet;
	res.m_data[0][1] = -(m_data[0][1] * det11 - m_data[0][2] * det10 + m_data[0][3] * det09) * invDet;
	res.m_data[0][2] =  (m_data[3][1] * det05 - m_data[3][2] * det04 + m_data[3][3] * det03) * invDet;
	res.m_data[0][3] = -(m_data[2][1] * det05 - m_data[2][2] * det04 + m_data[2][3] * det03) * invDet;

	res.m_data[1][0] = -(m_data[1][0] * det11 - m_data[1][2] * det08 + m_data[1][3] * det07) * invDet;
	res.m_data[1][1] =  (m_data[0][0] * det11 - m_data[0][2] * det08 + m_data[0][3] * det07) * invDet;
	res.m_data[1][2] = -(m_data[3][0] * det05 - m_data[3][2] * det02 + m_data[3][3] * det01) * invDet;
	res.m_data[1][3] =  (m_data[2][0] * det05 - m_data[2][2] * det02 + m_data[2][3] * det01) * invDet;

	res.m_data[2][0] =  (m_data[1][0] * det10 - m_data[1][1] * det08 + m_data[1][3] * det06) * invDet;
	res.m_data[2][1] = -(m_data[0][0] * det10 - m_data[0][1] * det08 + m_data[0][3] * det06) * invDet;
	res.m_data[2][2] =  (m_data[3][0] * det04 - m_data[3][1] * det02 + m_data[3][3] * det00) * invDet;
	res.m_data[2][3] = -(m_data[2][0] * det04 - m_data[2][1] * det02 + m_data[2][3] * det00) * invDet;

	res.m_data[3][0] = -(m_data[1][0] * det09 - m_data[1][1] * det07 + m_data[1][2] * det06) * invDet;
	res.m_data[3][1] =  (m_data[0][0] * det09 - m_data[0][1] * det07 + m_data[0][2] * det06) * invDet;
	res.m_data[3][2] = -(m_data[3][0] * det03 - m_data[3][1] * det01 + m_data[3][2] * det00) * invDet;
	res.m_data[3][3] =  (m_data[2][0] * det03 - m_data[2][1] * det01 + m_data[2][2] * det00) * invDet;

	return res;
}

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

const float* Mat4::operator[](int i) const noexcept
{
	GENESIS_ASSERT(i >= 0 && i < 4, "Index out of bounds.");
	return m_data[i];
}

float* Mat4::operator[](int i) noexcept
{
	GENESIS_ASSERT(i >= 0 && i < 4, "Index out of bounds.");
	return m_data[i];
}

Mat4 Mat4::operator+(const Mat4& rhs) const noexcept
{
	Mat4 res = *this;
	res += rhs;
	return res;
}

Mat4 Mat4::operator-(const Mat4& rhs) const noexcept
{
	Mat4 res = *this;
	res -= rhs;
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

Mat4& Mat4::operator+=(const Mat4& rhs) noexcept
{
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			m_data[row][col] += rhs.m_data[row][col];
		}
	}

	return *this;
}

Mat4& Mat4::operator-=(const Mat4& rhs) noexcept
{
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			m_data[row][col] -= rhs.m_data[row][col];
		}
	}

	return *this;
}

Mat4& Mat4::operator*=(const Mat4& rhs) noexcept
{
	*this = *this * rhs;
	return *this;
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
	float sinX = sinf(rotX);
	float cosX = cosf(rotX);

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
	float sinY = sinf(rotY);
	float cosY = cosf(rotY);

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
	float sinZ = sinf(rotZ);
	float cosZ = cosf(rotZ);

	res.m_data[0][0] = cosZ;
	res.m_data[0][1] = sinZ;
	res.m_data[1][0] = -sinZ;
	res.m_data[1][1] = cosZ;
	res.m_data[2][2] = 1;
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::orthographicLH(float width, float height, float znear, float zfar) noexcept
{
	Mat4 res{};

	res.m_data[0][0] = 2.0f / width;
	res.m_data[1][1] = 2.0f / height;
	res.m_data[2][2] = 1.0f / (zfar - znear);
	res.m_data[3][2] = -(znear / (zfar - znear));
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::orthographicOffCenterLH(float left, float right, float bottom, float top, float znear, float zfar) noexcept
{
	Mat4 res{};

	res.m_data[0][0] = 2.0f / (right - left);
	res.m_data[1][1] = 2.0f / (top - bottom);
	res.m_data[2][2] = 1.0f / (zfar - znear);
	res.m_data[3][0] = -(right + left) / (right - left);
	res.m_data[3][1] = -(top + bottom) / (top - bottom);
	res.m_data[3][2] = -(znear / (zfar - znear));
	res.m_data[3][3] = 1;

	return res;
}

Mat4 Mat4::perspectiveLH(float fov, float aspect, float znear, float zfar) noexcept
{
	Mat4 res{};
	float yscale = 1.0f / tanf(fov / 2.0f);
	float xscale = yscale / aspect;

	res.m_data[0][0] = xscale;
	res.m_data[1][1] = yscale;
	res.m_data[2][2] = zfar / (zfar - znear);
	res.m_data[2][3] = 1;
	res.m_data[3][2] = (-znear * zfar) / (zfar - znear);

	return res;
}

Mat4 Mat4::lookAtLH(const Vec3& eye, const Vec3& target, const Vec3& up) noexcept
{
	Mat4 res{};

	Vec3 zAxis = Vec3::normalize(target - eye);
	Vec3 xAxis = Vec3::normalize(Vec3::cross(up, zAxis));
	Vec3 yAxis = Vec3::cross(zAxis, xAxis);

	res[0][0] = xAxis.x;
	res[1][0] = xAxis.y;
	res[2][0] = xAxis.z;
	res[3][0] = -Vec3::dot(xAxis, eye);
	res[0][1] = yAxis.x;
	res[1][1] = yAxis.y;
	res[2][1] = yAxis.z;
	res[3][1] = -Vec3::dot(yAxis, eye);
	res[0][2] = zAxis.x;
	res[1][2] = zAxis.y;
	res[2][2] = zAxis.z;
	res[3][2] = -Vec3::dot(zAxis, eye);
	res[3][3] = 1;

	return res;
}