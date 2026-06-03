#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "Macros.h"
#include <math/Rect.h>

namespace constants
{
	using namespace genesis;

	// USER INTERFACE

	const Rect refCanvasSize = Rect{TARGET_RESOLUTION_WIDTH, TARGET_RESOLUTION_HEIGHT};

	const Vec4 fontColor = Vec4{1.0f, 1.0f, 1.0f, 1.0f};

	const Rect buttonSize = Rect{140, 50};
	const Vec4 buttonIdleColor = Vec4{0.1f, 0.1f, 0.1f, 1.0f};
	const Vec4 buttonHoverColor = Vec4{0.3f, 0.3f, 0.3f, 1.0f};

}

#endif