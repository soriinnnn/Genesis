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
        std::stable_sort(m_zOrdered.begin(), m_zOrdered.end(), 
            [](const RootElement* a, const RootElement* b) {
                return a->zOrder < b->zOrder;
            }
        );
        m_isZDirty = false;
    }
    for (auto* root : m_zOrdered) {
        auto* element = root->element.get();
        if (element->isEnabled()) {
            element->update(deltaTime);
        }
    }
}

void UIManager::render(SpriteBatch& batch)
{
    for (auto* root : m_zOrdered) {
        root->element->render(batch);
    }
}

void UIManager::destroyElement(const char* name)
{
    auto it = m_elements.find(name);
    if (it != m_elements.end()) {
        auto& root = it->second;
        if (root.element.get() == m_pressedElement) {
            m_pressedElement = nullptr;
        }

        auto newEnd = std::remove(m_zOrdered.begin(), m_zOrdered.end(), &root);
        m_zOrdered.erase(newEnd, m_zOrdered.end());
        m_elements.erase(it);
    }
}

void UIManager::setZOrder(const char* name, int zOrder)
{
    auto it = m_elements.find(name);
    if (it != m_elements.end()) {
        auto& root = it->second;
        root.zOrder = zOrder;
        m_isZDirty = true;
    }
}

void UIManager::onKeyDown(Key key) {}

void UIManager::onKeyUp(Key key) {}

void UIManager::onMouseMove(Point delta, Point pos) 
{
    bool hoverHandled = false;

    for (auto it = m_zOrdered.rbegin(); it != m_zOrdered.rend(); ++it) {
        auto& element = (*it)->element;    
        
        if (element->isEnabled() && element->isVisible() && element->contains(pos) && !hoverHandled) {
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
        auto& element = (*it)->element;

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
        m_pressedElement->m_pressed = false;;
    }
    m_pressedElement = nullptr;
}