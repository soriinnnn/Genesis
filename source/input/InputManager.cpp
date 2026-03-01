#include <input/InputManager.h>
#include <input/win32/Win32InputManager.h>
#include <algorithm>

using namespace genesis;
using namespace std;

InputManager::InputManager(const InputManagerDesc& desc): Base(desc.base) {}

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

UniquePtr<InputManager> InputManager::create(const InputManagerDesc& desc)
{
#ifdef _WIN32
	return make_unique<Win32InputManager>(desc);
#else
#error "Input manager creation is currently only supported on Windows."
#endif
}