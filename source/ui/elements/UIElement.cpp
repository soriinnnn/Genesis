#include <ui/elements/UIElement.h>
#include <ui/UIManager.h>
#include <cmath>

using namespace genesis;
using namespace std;

static Point calculateRelativeCenter(const Rect& size, const Rect& parentSize);

UIElement::UIElement(const UIElementDesc& desc): Base(desc.base), m_parent{desc.parent}
{
	m_position = Point{0, 0};
	m_margin = Point{0, 0};
	m_scale = Vec2{1.0f, 1.0f};
	m_color = Vec4{1.0f, 1.0f, 1.0f, 1.0f};
	m_size = Rect{0, 0};
	m_anchor = Anchor::Absolute;
	m_visible = true;
	m_enabled = true;
	m_hovered = false;
	m_pressed = false;
}

UIElement::~UIElement() {}

void UIElement::update(float deltaTime) 
{
	if (!isEnabled()) {
		return;
	}
	onUpdate(deltaTime);
}

void UIElement::render(SpriteBatch& batch)
{
	if (!isVisible()) {
		return;
	}
	onRender(batch);
}

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

Point UIElement::getMargin() const noexcept
{
	return m_margin;
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

Anchor UIElement::getAnchor() const noexcept
{
	return m_anchor;
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

void UIElement::setPosition(const Point& position)
{
	if (m_position == position) {
		return;
	}
	m_position = position;
	m_anchor = Anchor::Absolute;
	onPosition();
}

void UIElement::setScale(const Vec2& scale)
{
	if (m_scale == scale) {
		return;
	}
	m_scale = scale;
	onScale();
}

void UIElement::setColor(const Vec4& color)
{
	if (m_color == color) {
		return;
	}
	m_color = color;
	onColor();
}

void UIElement::setSize(const Rect& size)
{
	if (m_size == size) {
		return;
	}
	m_size = size;
	onSize();
	updateRelativeLayout();
}

void UIElement::setMargin(const Point& margin)
{
	if (!m_parent) {
		return;
	}
	if (m_margin == margin) {
		return;
	}
	m_margin = margin;
	updateRelativeLayout();
}

void UIElement::setAnchor(Anchor anchor)
{
	if (!m_parent) {
		return;
	}
	if (m_anchor == anchor) {
		return;
	}
	m_anchor = anchor;
	updateRelativeLayout();
}

void UIElement::setVisible(bool visible)
{
	if (m_visible == visible) {
		return;
	}
	m_visible = visible;
	if (!isVisible() && isHovered()) {
		onMouseOut();
	}
}

void UIElement::setEnabled(bool enabled)
{
	if (m_enabled == enabled) {
		return;
	}
	m_enabled = enabled;
	if (!isEnabled() && isHovered()) {
		onMouseOut();
	}
}

void UIElement::updateRelativeLayout()
{
	if (m_anchor == Anchor::Absolute) {
		return;
	}
	if (!m_parent) {
		return;
	}
	Rect parentSize = m_parent->getSize();

	switch (m_anchor) {
		case Anchor::TopLeft: {
			m_position = Point{
				m_margin.x,
				m_margin.y
			};
			break;
		}
		case Anchor::TopCenter: {
			Point relativeCenter = calculateRelativeCenter(m_size, parentSize);
			m_position = Point{
				relativeCenter.x + m_margin.x,
				m_margin.y
			};
			break;
		}
		case Anchor::TopRight: {
			m_position = Point{
				parentSize.width() - m_size.width() - m_margin.x,
				m_margin.y
			};
			break;
		}
		case Anchor::CenterLeft: {
			Point relativeCenter = calculateRelativeCenter(m_size, parentSize);
			m_position = Point{
				m_margin.x,
				relativeCenter.y + m_margin.y
			};
			break;
		}
		case Anchor::Center: {
			Point relativeCenter = calculateRelativeCenter(m_size, parentSize);
			m_position = Point{
				relativeCenter.x + m_margin.x,
				relativeCenter.y + m_margin.y
			};
			break;
		}
		case Anchor::CenterRight: {
			Point relativeCenter = calculateRelativeCenter(m_size, parentSize);
			m_position = Point{
				parentSize.width() - m_size.width() - m_margin.x,
				relativeCenter.y + m_margin.y
			};
			break;
		}
		case Anchor::BottomLeft: {
			m_position = Point{
				m_margin.x,
				parentSize.height() - m_size.height() - m_margin.y
			};
			break;
		}
		case Anchor::BottomCenter: {
			Point relativeCenter = calculateRelativeCenter(m_size, parentSize);
			m_position = Point{
				relativeCenter.x + m_margin.x,
				parentSize.height() - m_size.height() - m_margin.y
			};
			break;
		}
		case Anchor::BottomRight: {
			m_position = Point{
				parentSize.width() - m_size.width() - m_margin.x,
				parentSize.height() - m_size.height() - m_margin.y
			};
			break;
		}
	}

	onPosition();
}

void UIElement::onUpdate(float deltaTime) {}

void UIElement::onRender(SpriteBatch& batch) {}

void UIElement::onPosition() {}

void UIElement::onScale() {}

void UIElement::onColor() {}

void UIElement::onSize() {}

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

/* STATIC FUNCTION DEFINITIONS */

Point calculateRelativeCenter(const Rect& size, const Rect& parentSize)
{
	return Point{
		(parentSize.width() - size.width()) / 2,
		(parentSize.height() - size.height()) / 2
	};
}