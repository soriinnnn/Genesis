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
		void centerLabel() noexcept;
		
		UILabel* getLabel() noexcept;
		UIImage* getBackgroundImage() noexcept;

	private:
		void onScale() override;

	private:
		UniquePtr<UILabel> m_label;
		UniquePtr<UIImage> m_backgroundImage;
	};
}

#endif