#ifndef GENESIS_WIN32_INPUT_MANAGER_H
#define GENESIS_WIN32_INPUT_MANAGER_H
#include <input/InputManager.h>

#define KEYBOARD_STATE_SIZE 256

namespace genesis
{
	class Win32InputManager final: public InputManager
	{
	public:
		explicit Win32InputManager(const InputManagerDesc& desc);
		~Win32InputManager() override;

		void update() override;

		bool isKeyDown(Key key) const noexcept override;
		bool isKeyUp(Key key) const noexcept override;
		bool isKeyPressed(Key key) const noexcept override;
		bool isKeyReleased(Key key) const noexcept override;
		bool isMouseDown(MouseButton button) const noexcept override;
		bool isMouseUp(MouseButton button) const noexcept override;
		bool isMousePressed(MouseButton button) const noexcept override;
		bool isMouseReleased(MouseButton button) const noexcept override;
		Point getMousePosition() const noexcept override;
		Point getMouseDelta() const noexcept override;

	private:
		void updateKeyboard();
		void updateMouse();

	private:
		uint8 m_currentKeys[KEYBOARD_STATE_SIZE];
		uint8 m_previousKeys[KEYBOARD_STATE_SIZE];
		Point m_currentMousePos;
		Point m_previousMousePos;
		bool m_firstFrame;
	};
}

#endif