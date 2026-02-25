#include <math/Mat4.h>
#include <cmath>

using namespace genesis;
using namespace std;

Mat4::Mat4(): data({}) {}

Mat4 Mat4::operator*(const Mat4& rhs) const noexcept
{
	Mat4 res{};

	for (int row = 0; row < 4; row++) {
		for (int k = 0; k < 4; k++) {
			float temp = data[row][k];

			for (int col = 0; col < 4; col++) {
				res.data[row][col] += temp * rhs.data[k][col];
			}
		}
	}

	return res;
}

Mat4 Mat4::identity() noexcept
{
	Mat4 res{};
	res.data[0][0] = 1;
	res.data[1][1] = 1;
	res.data[2][2] = 1;
	res.data[3][3] = 1;

	return res;
}

Mat4 Mat4::fromTranslation(const Vec3& translation) noexcept
{
	Mat4 res = identity();
	res.data[3][0] = translation.x;
	res.data[3][1] = translation.y;
	res.data[3][2] = translation.z;

	return res;
}

Mat4 Mat4::fromScale(const Vec3& scale) noexcept
{
	Mat4 res{};
	res.data[0][0] = scale.x;
	res.data[1][1] = scale.y;
	res.data[2][2] = scale.z;
	res.data[3][3] = 1;

	return res;
}

Mat4 Mat4::fromRotation(const Vec3& rotation) noexcept
{
	return Mat4();
}

Mat4 Mat4::fromRotationX(float rotX) noexcept
{
	Mat4 res{};
	float sinX = sin(rotX);
	float cosX = cos(rotX);
	/*
	res.data[0][0] = 1;
	res.data[1][1] = cosX;
	res.data[1][2] = sinX;
	res.data[2][1] = -sinX;
	res.data[2][2] = cosX;
	res.data[3][3] = 1;
	*/
	return res;
}

Mat4 Mat4::fromRotationY(float rotY) noexcept
{
	Mat4 res{};
	float sinY = sin(rotY);
	float cosY = cos(rotY);

	

	return res;
}

Mat4 Mat4::fromRotationZ(float rotZ) noexcept
{
	Mat4 res{};
	float sinZ = sin(rotZ);
	float cosZ = cos(rotZ);

	res.data[0][0] = cosZ;
	res.data[0][1] = sinZ;
	res.data[1][0] = -sinZ;
	res.data[1][1] = cosZ;
	res.data[2][2] = 1;
	res.data[3][3] = 1;

	return res;
}
