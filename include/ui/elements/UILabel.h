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

		void render(SpriteBatch& batch) override;

		void setContent(const char* content);
		void setFont(SharedPtr<Font> font);

	private:
		void updateSize();

	private:
		String m_content;
		SharedPtr<Font> m_font;
	};
}

#endif