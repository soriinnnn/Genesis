#include <ui/elements/UILabel.h>
#include <resources/Font.h>
#include <graphics/resources/FontAtlas.h>
#include <graphics/resources/SpriteBatch.h>

using namespace genesis;
using namespace std;

UILabel::UILabel(const UIElementDesc& desc): UIElement(desc) {}

UILabel::~UILabel() {}

void UILabel::render(SpriteBatch& batch) 
{
	if (!m_font) {
		return;
	}
	batch.drawText(m_font->getFontAtlas(), m_content.c_str(), m_position, m_color);
}

void UILabel::setContent(const char* content)
{
	m_content = content;
	updateBounds();
}

void UILabel::setColor(Vec4 color)
{
	m_color = color;
}

void UILabel::setFont(SharedPtr<Font> font)
{
	m_font = font;
	updateBounds();
}

void UILabel::updateBounds()
{
	if (!m_font) {
		m_bounds = Rect{};
		return;
	}
	m_bounds = m_font->getFontAtlas().getBounds(m_content.c_str(), m_position);
}