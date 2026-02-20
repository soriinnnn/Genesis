#ifndef GENESIS_RECT_H
#define GENESIS_RECT_H
#include <core/Core.h>

namespace genesis 
{
	class Rect
	{
	public:
		Rect();
		Rect(uint32 width, uint32 height);
		Rect(uint32 left, uint32 top, uint32 right, uint32 bottom);

		uint32 width() const noexcept;
		uint32 height() const noexcept;

	public:
		uint32 left, top, right, bottom;
	};
}

#endif