#ifndef UTILS_H
#define UTILS_H
#include <math/Vec2.h>
#include <math/Rect.h>
#include <math/Point.h>

namespace utils
{
	using namespace genesis;

	// pre: referenceSize.width() > 0, referenceSize.height() > 0
	inline Vec2 getSizeScaleRatio(Rect currentSize, Rect referenceSize)
	{
		return Vec2{
			static_cast<float>(currentSize.width()) / referenceSize.width(),
			static_cast<float>(currentSize.height()) / referenceSize.height()
		};
	}

	inline Point applyMarginScale(Point baseMargin, Vec2 scaleRatio)
	{
		return Point{
			static_cast<int32>(std::round(baseMargin.x * scaleRatio.x)),
			static_cast<int32>(std::round(baseMargin.y * scaleRatio.y))
		};
	}
}

#endif