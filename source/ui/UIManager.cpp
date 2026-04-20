#include <ui/UIManager.h>
#include <ui/elements/UIElement.h>
#include <graphics/GraphicsDevice.h>
#include <algorithm>

using namespace genesis;

UIManager::UIManager(const UIManagerDesc& desc): Base(desc.base), m_pressedElement{nullptr}, m_isZDirty{false} {}

UIManager::~UIManager() {}

void UIManager::update(float deltaTime)
{
    if (m_isZDirty) {
        std::stable_sort(m_zOrdered.begin(), m_zOrdered.end(), [](const UIElement* a, const UIElement* b) {return a->getZOrder() < b->getZOrder();});
        m_isZDirty = false;
    }

    for (auto* element : m_zOrdered) {
        if (element->isEnabled()) {
            element->update(deltaTime);
        }
    }
}

void UIManager::destroyElement(const char* name)
{
    auto it = m_elements.find(name);
    if (it != m_elements.end()) {
        if (it->second.get() == m_pressedElement) {
            m_pressedElement = nullptr;
        }
        m_zOrdered.erase(std::remove(m_zOrdered.begin(), m_zOrdered.end(), it->second.get()), m_zOrdered.end());
        m_elements.erase(it);
    }
}

void UIManager::onKeyDown(Key key) {}

void UIManager::onKeyUp(Key key) {}

void UIManager::onMouseMove(Point delta, Point pos) 
{
    bool hoverHandled = false;

    for (auto it = m_zOrdered.rbegin(); it != m_zOrdered.rend(); ++it) {
        auto* element = *it;    

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
    for (auto it = m_zOrdered.rbegin(); it != m_zOrdered.rend(); ++it) {
        auto* element = *it;

        if (!element->isEnabled() || !element->isVisible()) {
            continue;
        }

        if (element->contains(pos)) {
            m_pressedElement = element;
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