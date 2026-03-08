#include <math/Vec4.h>

using namespace genesis;

Vec4::Vec4() 
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

Vec4::Vec4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

const float* Vec4::toArray() const noexcept
{
	return &this->x;
}