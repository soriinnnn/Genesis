#include <math/Point.h>

using namespace genesis;

Point::Point()
{
	this->x = 0;
	this->y = 0;
}

Point::Point(int32 x, int32 y)
{
	this->x = x;
	this->y = y;
}

Point Point::operator-(const Point& rhs) const noexcept
{
	return Point{this->x - rhs.x, this->y - rhs.y};
}

bool Point::operator==(const Point& rhs) const noexcept
{
	return this->x == rhs.x && this->y == rhs.y;
}
