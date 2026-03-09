#ifndef GENESIS_INPUT_MANAGER_H
#define GENESIS_INPUT_MANAGER_H
#include <core/Base.h>
#include <core/Core.h>
#include <display/Window.h>
#include <input/InputListener.h>

namespace genesis
{
	struct InputManagerDesc
	{
		BaseDesc base;
		Window& wnd;
	};

	class InputManager: public Base
	{
	public:
		virtual ~InputManager() override;

		virtual void update() = 0;
		void addListener(InputListener* listener);
		void removeListener(InputListener* listener);

		virtual bool isKeyDown(Key key) const = 0;
		virtual bool isKeyUp(Key key) const = 0;
		virtual bool isKeyPressed(Key key) const = 0;
		virtual bool isKeyReleased(Key key) const = 0;
		virtual bool isMouseDown(MouseButton button) const = 0;
		virtual bool isMouseUp(MouseButton button) const = 0;
		virtual bool isMousePressed(MouseButton button) const = 0;
		virtual bool isMouseReleased(MouseButton button) const = 0;
		virtual Point getMousePosition() const = 0;
		virtual Point getMouseDelta() const = 0;

		virtual void setMousePosition(Point pos) = 0;
		virtual void setMouseVisibility(bool visible) = 0;

		static UniquePtr<InputManager> create(const InputManagerDesc& desc);

	protected:
		explicit InputManager(const InputManagerDesc& desc);

	protected:
		Window& m_window;
		std::vector<InputListener*> m_listeners;
	};
}

#endif