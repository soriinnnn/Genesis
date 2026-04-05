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
		void setColor(Vec4 color);
		void setFont(SharedPtr<Font> font);

	private:
		void updateBounds();

	private:
		String m_content;
		Vec4 m_color;
		SharedPtr<Font> m_font;
	};
}

#endif