#ifndef GENESIS_UI_LABEL_H
#define GENESIS_UI_LABEL_H
#include <ui/elements/UIElement.h>
#include <math/Vec4.h>

namespace genesis
{
	class UILabel final: public UIElement
	{
	public:
		explicit UILabel(const UIElementDesc& desc);
		~UILabel() override;

		void setContent(const char* content);
		void setFont(const SharedPtr<Font>& font);

	private:
		void adjustContent();

		void onRender(SpriteBatch& batch) override;
		void onSize() override;

	private:
		WString m_content;
		WString m_adjustedContent;
		SharedPtr<Font> m_font;
	};
}

#endif