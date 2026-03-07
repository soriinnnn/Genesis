#ifndef GENESIS_POINT2_H
#define GENESIS_POINT2_H
#include <core/Core.h>

namespace genesis
{
	class Point
	{
	public:
		Point();
		Point(int32 x, int32 y);

	public:
		int32 x, y;
	};
}

#endif