#ifndef GENESIS_UI_PANEL_H
#define GENESIS_UI_PANEL_H
#include <ui/elements/UIElement.h>

GENESIS_NAMESPACE_BEGIN

class UIPanel final: public UIElement
{
public:
	explicit UIPanel(const UIElementDesc& desc);
	~UIPanel() override;

private:
	void onRender(SpriteBatch& batch) override;
};

GENESIS_NAMESPACE_END

#endif