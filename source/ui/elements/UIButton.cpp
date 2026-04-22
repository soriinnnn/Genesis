#include <ui/elements/UIButton.h>
#include <ui/elements/UILabel.h>
#include <ui/elements/UIImage.h>

using namespace genesis;
using namespace std;

UIButton::UIButton(const UIElementDesc& desc): UIElement(desc)
{
	m_label = make_unique<UILabel>(UIElementDesc{m_logger, this});
	m_backgroundImage = make_unique<UIImage>(UIElementDesc{m_logger, this});
	m_size = Rect{1, 1};
}

UIButton::~UIButton() {}

void UIButton::render(SpriteBatch& batch)
{
	m_backgroundImage->render(batch);
	m_label->render(batch);
}

void UIButton::centerLabel() noexcept
{
    Rect parentBounds = getBounds();
    Rect labelBounds = m_label->getBounds();

    int parentWidth = parentBounds.right - parentBounds.left;
    int parentHeight = parentBounds.bottom - parentBounds.top;
    int labelWidth = labelBounds.right - labelBounds.left;
    int labelHeight = labelBounds.bottom - labelBounds.top;

    m_label->setPosition({((parentWidth - labelWidth) / 2) - 2, ((parentHeight - labelHeight) / 2) + 3});
}

UILabel* UIButton::getLabel() noexcept
{
	return m_label.get();
}

UIImage* UIButton::getBackgroundImage() noexcept
{
	return m_backgroundImage.get();
}

void UIButton::onScale()
{
	m_backgroundImage->setScale(m_scale);
}
