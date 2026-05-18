#ifndef GENESIS_UI_ELEMENT_H
#define GENESIS_UI_ELEMENT_H
#include <core/Base.h>
#include <input/InputCodes.h>
#include <math/Rect.h>
#include <math/Point.h>
#include <math/Vec4.h>
#include <math/Vec2.h>

namespace genesis
{
	struct UIAnchor
	{
		enum class Horizontal {Left, Center, Right};
		enum class Vertical {Top, Center, Bottom};

		Horizontal horizontal = Horizontal::Left;
		Vertical vertical = Vertical::Top;
		Rect margin{};
		float widthPercent = 0.0f;
		float heightPercent = 0.0f;
		float fixedWidth = 200.0f;
		float fixedHeight = 100.0f;
	};

	struct UIElementDesc
	{
		BaseDesc base;
		UIElement* parent = nullptr;
	};

	class UIElement: public Base
	{
	public:
		virtual ~UIElement() override;

		void update(float deltaTime);
		void render(SpriteBatch& batch);

		bool contains(const Point& position) const noexcept;

		Point getGlobalPosition() const noexcept;
		Point getPosition() const noexcept;
		Vec2 getScale() const noexcept;
		Vec4 getColor() const noexcept;
		Rect getSize() const noexcept;
		Rect getBounds() const noexcept;
		bool isVisible() const noexcept;
		bool isEnabled() const noexcept;
		bool isHovered() const noexcept;
		bool isPressed() const noexcept;

		void setPosition(const Point& position) noexcept;
		void setScale(const Vec2& scale) noexcept;
		void setColor(const Vec4& color) noexcept;
		void setSize(const Rect& size) noexcept;
		void setVisible(bool visible);
		void setEnabled(bool enabled);

		void setOnMouseDownCallback(std::function<void(MouseButton)> callback) noexcept;
		void setOnMouseUpCallback(std::function<void(MouseButton)> callback) noexcept;
		void setOnMouseEnterCallback(std::function<void()> callback) noexcept;
		void setOnMouseOutCallback(std::function<void()> callback) noexcept;

	protected:
		explicit UIElement(const UIElementDesc& desc);

		virtual void onUpdate(float deltaTime);
		virtual void onRender(SpriteBatch& batch);
		virtual void onPosition();
		virtual void onScale();
		virtual void onColor();
		virtual void onSize();

		virtual void onMouseDown(MouseButton button);
		virtual void onMouseUp(MouseButton button);
		virtual void onMouseEnter();
		virtual void onMouseOut();

	protected:
		UIElement* m_parent;
		Point m_position;
		Vec2 m_scale;
		Vec4 m_color;
		Rect m_size;
		bool m_visible;
		bool m_enabled;
		bool m_hovered;
		bool m_pressed;

	private:
		std::function<void(MouseButton)> m_onMouseDown;
		std::function<void(MouseButton)> m_onMouseUp;
		std::function<void()> m_onMouseEnter;
		std::function<void()> m_onMouseOut;

		friend class UIManager;
	};
}

#endif