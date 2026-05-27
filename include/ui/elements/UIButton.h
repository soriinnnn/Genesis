#ifndef GENESIS_UI_BUTTON_H
#define GENESIS_UI_BUTTON_H
#include <ui/elements/UIElement.h>

GENESIS_NAMESPACE_BEGIN

class UIButton: public UIElement
{
public:
	explicit UIButton(const UIElementDesc& desc);
	~UIButton() override;

	UILabel* getLabel() noexcept;

private:
	void onRender(SpriteBatch& batch) override;
	void onPosition() override;

private:
	UniquePtr<UILabel> m_label;
};

GENESIS_NAMESPACE_END

#endif