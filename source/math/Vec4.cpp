#include <math/Vec4.h>

using namespace genesis;

Vec4::Vec4() 
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

Vec4::Vec4(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = 0;
}

Vec4::Vec4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4::Vec4(const Vec3& v, float w)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}

bool Vec4::operator==(const Vec4& rhs) const noexcept
{
	return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w;
}

const float* Vec4::toArray() const noexcept
{
	return &this->x;
}