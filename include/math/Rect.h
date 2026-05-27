#ifndef GENESIS_RECT_H
#define GENESIS_RECT_H
#include <core/Core.h>

GENESIS_NAMESPACE_BEGIN

class Rect
{
public:
	Rect();
	Rect(int32 width, int32 height);
	Rect(int32 left, int32 top, int32 right, int32 bottom);

	int32 width() const noexcept;
	int32 height() const noexcept;

	bool operator==(const Rect& rhs) const noexcept;

public:
	int32 left, top, right, bottom;
};

GENESIS_NAMESPACE_END

#endif