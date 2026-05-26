#ifndef GENESIS_UI_WINDOW_H
#define GENESIS_UI_WINDOW_H
#include <ui/elements/UIElement.h>

GENESIS_NAMESPACE_START

class UICanvas final: public UIElement
{
public:
	explicit UICanvas(const UIElementDesc& desc);
	~UICanvas() override;
};

GENESIS_NAMESPACE_END

#endif