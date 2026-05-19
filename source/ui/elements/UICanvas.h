#ifndef GENESIS_UI_WINDOW_H
#define GENESIS_UI_WINDOW_H
#include <ui/elements/UIElement.h>

namespace genesis
{
	class UICanvas final: public UIElement
	{
	public:
		explicit UICanvas(const UIElementDesc& desc);
		~UICanvas() override;
	};
}

#endif