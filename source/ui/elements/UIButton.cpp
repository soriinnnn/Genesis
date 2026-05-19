#include <ui/elements/UIButton.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIPanel.h>
#include <graphics/resources/SpriteBatch.h>

using namespace genesis;
using namespace std;

UIButton::UIButton(const UIElementDesc& desc): UIElement(desc)
{
	m_label = make_unique<UILabel>(UIElementDesc{m_logger, this});
}

UIButton::~UIButton() {}

UILabel* UIButton::getLabel() noexcept
{
	return m_label.get();
}

void UIButton::onRender(SpriteBatch& batch)
{
    batch.drawSolid(m_size, getGlobalPosition(), m_scale, m_color);
    m_label->render(batch);
}