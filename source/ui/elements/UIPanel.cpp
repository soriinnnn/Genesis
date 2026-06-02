#include <ui/elements/UIPanel.h>
#include <graphics/resources/SpriteBatch.h>

using namespace genesis;

UIPanel::UIPanel(const UIElementDesc& desc): UIElement(desc) 
{
	m_color = Vec4{0.0f, 0.0f, 0.0f, 1.0f};
}

UIPanel::~UIPanel() {}

void UIPanel::onRender(SpriteBatch& batch)
{
	batch.drawSolid(m_size, getGlobalPosition(), m_scale, m_color);
}

void UIPanel::onSize()
{
	m_size = Rect{
		static_cast<int32>(round(m_size.width() * m_scale.x)),
		static_cast<int32>(round(m_size.height() * m_scale.y))
	};
}

void UIPanel::onScale()
{
	m_size = Rect{
		static_cast<int32>(round(m_size.width() * m_scale.x)),
		static_cast<int32>(round(m_size.height() * m_scale.y))
	};
	updateRelativeLayout();
}