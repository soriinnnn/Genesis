#include <math/Vec2.h>

using namespace genesis;

Vec2::Vec2()
{
	this->x = 0;
	this->y = 0;
}

genesis::Vec2::Vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

const float* genesis::Vec2::toArray() const noexcept
{
	return &this->x;
}
