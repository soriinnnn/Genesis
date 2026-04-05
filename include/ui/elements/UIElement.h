#ifndef GENESIS_UI_ELEMENT_H
#define GENESIS_UI_ELEMENT_H
#include <core/Base.h>
#include <input/InputCodes.h>
#include <math/Rect.h>
#include <math/Point.h>

namespace genesis
{
	struct UIElementDesc
	{
		BaseDesc base;
	};

	class UIElement: public Base
	{
	public:
		virtual ~UIElement() override;

		virtual void update(float deltaTime);
		virtual void render(SpriteBatch& batch) = 0;

		void release() noexcept;
		bool contains(Point position) const noexcept;

		Rect getBounds() const noexcept;
		Point getPosition() const noexcept;
		bool isVisible() const noexcept;
		bool isEnabled() const noexcept;
		bool isHovered() const noexcept;
		bool isPressed() const noexcept;

		void setPosition(Point position) noexcept;
		void setVisible(bool visible);
		void setEnabled(bool enabled);

		void setOnMouseDownCallback(std::function<void(MouseButton)> callback) noexcept;
		void setOnMouseUpCallback(std::function<void(MouseButton)> callback) noexcept;
		void setOnMouseEnterCallback(std::function<void()> callback) noexcept;
		void setOnMouseOutCallback(std::function<void()> callback) noexcept;

	protected:
		explicit UIElement(const UIElementDesc& desc);

		virtual void onMouseDown(MouseButton button);
		virtual void onMouseUp(MouseButton button);
		virtual void onMouseEnter();
		virtual void onMouseOut();

	protected:
		Point m_position;
		Rect m_bounds;
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