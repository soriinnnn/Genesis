#include <input/InputManager.h>
#include <input/win32/Win32InputManager.h>
#include <algorithm>

using namespace genesis;
using namespace std;

InputManager::InputManager(const InputManagerDesc& desc): Base(desc.base), m_window{desc.window}
{
	m_mouseVisible = true;
	m_mouseLocked = false;
	m_ignoreNextMouseMove = false;
}

InputManager::~InputManager() {}

void InputManager::addListener(InputListener* listener)
{
	auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it == m_listeners.end()) {
		m_listeners.push_back(listener);
	}
}

void InputManager::removeListener(InputListener* listener)
{
	auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it != m_listeners.end()) {
		*it = m_listeners.back();
		m_listeners.pop_back();
	}
}

bool InputManager::isMouseVisible() const noexcept
{
	return m_mouseVisible;
}

bool InputManager::isMouseLocked() const noexcept
{
	return m_mouseLocked;
}

void InputManager::ignoreNextMouseMove() noexcept
{
	m_ignoreNextMouseMove = true;
}

UniquePtr<InputManager> InputManager::create(const InputManagerDesc& desc)
{
#ifdef _WIN32
	return make_unique<Win32InputManager>(desc);
#else
#error "InputManager creation is currently only supported on Windows."
#endif
}