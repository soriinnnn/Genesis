#include <ui/UIManager.h>
#include <ui/elements/UIElement.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;

UIManager::UIManager(const UIManagerDesc& desc): Base(desc.base), m_pressedElement{nullptr}  {}

UIManager::~UIManager() {}

void UIManager::update(float deltaTime)
{
    for (auto& element : m_elements) {
        if (element->isEnabled()) {
            element->update(deltaTime);
        }
    }
}

void UIManager::destroyElement(UIElement* element)
{
    auto it = std::find_if(m_elements.begin(), m_elements.end(),
        [element](const UniquePtr<UIElement>& e) {
            return e.get() == element;
        }
    );

    if (it != m_elements.end()) {
        if (it->get() == m_pressedElement) {
            m_pressedElement = nullptr;
        }
        m_elements.erase(it);
    }
}

void UIManager::onKeyDown(Key key) {}

void UIManager::onKeyUp(Key key) {}

void UIManager::onMouseMove(Point delta, Point pos) 
{
    bool hoverHandled = false;

    for (auto it = m_elements.rbegin(); it != m_elements.rend(); ++it) {
        auto& element = *it;    

        if (!element->isEnabled() || !element->isVisible()) {
            continue;
        }

        if (element->contains(pos) && !hoverHandled) {
            if (!element->isHovered()) {
                element->onMouseEnter();
            }
            hoverHandled = true;
        }
        else {
            if (element->isHovered()) {
                element->onMouseOut();
            }
        }
    }
}

void UIManager::onMouseDown(MouseButton button, Point pos) 
{
    for (auto it = m_elements.rbegin(); it != m_elements.rend(); ++it) {
        auto& element = *it;

        if (!element->isEnabled() || !element->isVisible()) {
            continue;
        }

        if (element->contains(pos)) {
            m_pressedElement = element.get();
            element->onMouseDown(button);
            break;
        }
    }
}

void UIManager::onMouseUp(MouseButton button, Point pos) 
{
    if (!m_pressedElement) {
        return;
    }

    if (m_pressedElement->contains(pos)) {
        m_pressedElement->onMouseUp(button);
    }
    else {
        m_pressedElement->release();
    }
    m_pressedElement = nullptr;
}