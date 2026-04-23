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

void UIButton::render(SpriteBatch& batch)
{
	batch.drawSolid(m_size, getGlobalPosition(), m_scale, m_color);
	m_label->render(batch);
}

UILabel* UIButton::getLabel() noexcept
{
	return m_label.get();
}

void UIButton::centerLabel() noexcept
{
    Rect parentBounds = getBounds();
    Rect labelBounds = m_label->getBounds();

    int parentWidth = parentBounds.right - parentBounds.left;
    int parentHeight = parentBounds.bottom - parentBounds.top;
    int labelWidth = labelBounds.right - labelBounds.left;
    int labelHeight = labelBounds.bottom - labelBounds.top;
    m_label->setPosition({((parentWidth - labelWidth) / 2), ((parentHeight - labelHeight) / 2)});
}