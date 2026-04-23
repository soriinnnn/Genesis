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
	batch.drawText(m_font->getFontAtlas(), m_content.c_str(), getGlobalPosition(), m_scale, m_color);
}

void UILabel::setContent(const char* content)
{
	m_content = content;
	updateSize();
}

void UILabel::setFont(SharedPtr<Font> font)
{
	m_font = font;
	updateSize();
}

void UILabel::setSize(Rect size) noexcept
{
	GENESIS_LOG_INFO("Label cannot be resized manually.");
}

void UILabel::updateSize()
{
	if (!m_font) {
		m_size = Rect{};
		return;
	}
	m_size = m_font->getFontAtlas().getSize(m_content.c_str());
}