#include <core/Core.h>
#include <ui/elements/UIElement.h>

namespace genesis
{
	Point getCenteredPosition(const UIElement& element, const Rect& rect)
	{
		Rect elementSize = element.getSize();
		return Point{
			(rect.width() - elementSize.width()) / 2,
			(rect.height() - elementSize.height()) / 2
		};
	}
}