#ifndef GENESIS_POINT2_H
#define GENESIS_POINT2_H
#include <core/Core.h>

GENESIS_NAMESPACE_START

class Point
{
public:
	Point();
	Point(int32 x, int32 y);

	Point operator-(const Point& rhs) const noexcept;
	bool operator==(const Point& rhs) const noexcept;

public:
	int32 x, y;
};

GENESIS_NAMESPACE_END

#endif