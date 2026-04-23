#ifndef GENESIS_UI_BUTTON_H
#define GENESIS_UI_BUTTON_H
#include <ui/elements/UIElement.h>

namespace genesis
{
	class UIButton: public UIElement
	{
	public:
		explicit UIButton(const UIElementDesc& desc);
		~UIButton() override;

		void render(SpriteBatch& batch) override;
		
		UILabel* getLabel() noexcept;
		void centerLabel() noexcept;

	private:
		UniquePtr<UILabel> m_label;
	};
}

#endif