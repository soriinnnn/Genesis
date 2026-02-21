#include <math/Vec3.h>

using namespace genesis;

Vec3::Vec3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vec3::Vec3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

const float* Vec3::toArray() const noexcept
{
    return &this->x;
}
