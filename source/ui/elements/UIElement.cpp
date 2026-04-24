#include <ui/elements/UIElement.h>
#include <ui/UIManager.h>

using namespace genesis;

UIElement::UIElement(const UIElementDesc& desc): Base(desc.base), m_parent{desc.parent}
{
	m_scale = Vec2{1.0f, 1.0f};
	m_color = Vec4{1.0f, 1.0f, 1.0f, 1.0f};
	m_visible = true;
	m_enabled = true;
	m_hovered = false;
	m_pressed = false;
}

UIElement::~UIElement() {}

void UIElement::update(float deltaTime) {}

bool UIElement::contains(const Point& point) const noexcept
{
	Rect bounds = getBounds();
	return point.x >= bounds.left && 
		   point.x <= bounds.right &&
		   point.y >= bounds.top && 
		   point.y <= bounds.bottom;
}

Point UIElement::getGlobalPosition() const noexcept
{
	if (m_parent) {
		Point parentPos = m_parent->getGlobalPosition();
		return Point{parentPos.x + m_position.x, parentPos.y + m_position.y};
	}
	return m_position;
}

Point UIElement::getPosition() const noexcept
{
	return m_position;
}

Vec2 UIElement::getScale() const noexcept
{
	return m_scale;
}

Vec4 UIElement::getColor() const noexcept
{
	return m_color;
}

Rect UIElement::getSize() const noexcept
{
	return m_size;
}

Rect UIElement::getBounds() const noexcept
{
	Point globalPos = getGlobalPosition();
	return Rect{
		globalPos.x,
		globalPos.y,
		globalPos.x + static_cast<int32>(m_size.width() * m_scale.x),
		globalPos.y + static_cast<int32>(m_size.height() * m_scale.y)
	};
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

void UIElement::setPosition(const Point& position) noexcept
{
	m_position = position;
}

void UIElement::setScale(const Vec2& scale) noexcept
{
	m_scale = scale;
}

void UIElement::setColor(const Vec4& color) noexcept
{
	m_color = color;
}

void UIElement::setSize(const Rect& size) noexcept
{
	m_size = size;
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

void UIElement::onPosition() {}

void UIElement::onScale() {}

void UIElement::onColor() {}

void UIElement::onSize() {}

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