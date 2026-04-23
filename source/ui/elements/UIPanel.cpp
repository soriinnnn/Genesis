#include <ui/elements/UIPanel.h>
#include <graphics/resources/SpriteBatch.h>

using namespace genesis;

UIPanel::UIPanel(const UIElementDesc& desc): UIElement(desc) 
{
	m_color = Vec4{0.0f, 0.0f, 0.0f, 1.0f};
}

UIPanel::~UIPanel() {}

void UIPanel::render(SpriteBatch& batch) 
{
	batch.drawSolid(m_size, getGlobalPosition(), m_scale, m_color);
}