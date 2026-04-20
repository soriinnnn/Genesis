#include <ui/elements/UIElement.h>
#include <ui/UIManager.h>

using namespace genesis;

UIElement::UIElement(const UIElementDesc& desc): Base(desc.base), m_manager{desc.manager}
{
	m_zOrder = 0;
	m_visible = true;
	m_enabled = true;
	m_hovered = false;
	m_pressed = false;
}

UIElement::~UIElement() {}

void UIElement::update(float deltaTime) {}

void UIElement::release() noexcept
{
	m_pressed = false;
}

bool UIElement::contains(Point point) const noexcept
{
	return point.x >= m_position.x && point.x <= m_position.x + m_bounds.width() &&
		   point.y >= m_position.y && point.y <= m_position.y + m_bounds.height();
}

Rect UIElement::getBounds() const noexcept
{
	return m_bounds;
}

Point UIElement::getPosition() const noexcept
{
	return m_position;
}

int UIElement::getZOrder() const noexcept
{
	return m_zOrder;
}

bool UIElement::isVisible() const noexcept
{
	return m_visible;
}

bool UIElement::isEnabled() const noexcept
{
	return m_enabled;
}

bool UIElement::isHovered() const noexcept
{
	return m_hovered;
}

bool UIElement::isPressed() const noexcept
{
	return m_pressed;
}

void UIElement::setZOrder(int zOrder) noexcept
{
	m_zOrder = zOrder;
	m_manager.m_isZDirty = true;
}

void UIElement::setPosition(Point position) noexcept
{
	m_position = position;
}

void UIElement::setVisible(bool visible)
{
	m_visible = visible;
	if (!isVisible() && isHovered()) {
		onMouseOut();
	}
}

void UIElement::setEnabled(bool enabled)
{
	m_enabled = enabled;
	if (!isEnabled() && isHovered()) {
		onMouseOut();
	}
}

void UIElement::onMouseDown(MouseButton button)
{
	if (!isEnabled() || !isVisible()) {
		return;
	}
	m_pressed = true;
	if (m_onMouseDown) {
		m_onMouseDown(button);
	}
}

void UIElement::onMouseUp(MouseButton button)
{
	if (!isEnabled() || !isVisible() || !isPressed()) {
		return;
	}
	m_pressed = false;
	if (m_onMouseUp) {
		m_onMouseUp(button);
	}
}

void UIElement::onMouseEnter()
{
	if (!isEnabled() || !isVisible()) {
		return;
	}
	m_hovered = true;
	if (m_onMouseEnter) {
		m_onMouseEnter();
	}
}

void UIElement::onMouseOut()
{
	if (!isEnabled() || !isVisible()) {
		return;
	}
	m_hovered = false;
	if (m_onMouseOut) {
		m_onMouseOut();
	}
}

void UIElement::setOnMouseDownCallback(std::function<void(MouseButton)> callback) noexcept
{
	m_onMouseDown = callback;
}

void UIElement::setOnMouseUpCallback(std::function<void(MouseButton)> callback) noexcept
{
	m_onMouseUp = callback;
}

void UIElement::setOnMouseEnterCallback(std::function<void()> callback) noexcept
{
	m_onMouseEnter = callback;
}

void UIElement::setOnMouseOutCallback(std::function<void()> callback) noexcept
{
	m_onMouseOut = callback;
}
