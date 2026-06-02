#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "Macros.h"
#include <math/Rect.h>

namespace constants
{
	using namespace genesis;

	// USER INTERFACE

	const Rect referenceSize = Rect{TARGET_RESOLUTION_WIDTH, TARGET_RESOLUTION_HEIGHT};
	const Point earthMargin = Point{200, -100};

}

#endif