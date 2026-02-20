#ifndef GENESIS_RECT_H
#define GENESIS_RECT_H

namespace genesis 
{
	class Rect
	{
	public:
		Rect();
		Rect(int width, int height);
		Rect(int left, int top, int right, int bottom);

		int width() const noexcept;
		int height() const noexcept;

	public:
		int left, top, right, bottom;
	};
}

#endif