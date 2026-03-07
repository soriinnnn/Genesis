#include <window/Window.h>
#include <window/win32/Win32Window.h>

using namespace genesis;

Window::Window(const WindowDesc& desc): Base(desc.base)
{
	m_handle = nullptr;
	m_size = desc.size;
	m_onResize = nullptr;
	m_onFocus = nullptr;
	m_onKillFocus = nullptr;
	m_hasFocus = true;
}

Window::~Window() {}

void* Window::getHandle() const noexcept
{
	return m_handle;
}

Rect Window::getSize() const noexcept
{
	return m_size;
}

bool Window::hasFocus() const noexcept
{
	return m_hasFocus;
}

void Window::onResize(std::function<void(uint32, uint32)> callback)
{
	m_onResize = callback;
}

void Window::onFocus(std::function<void()> callback)
{
	m_onFocus = callback;
}

void Window::onKillFocus(std::function<void()> callback)
{
	m_onKillFocus = callback;
}

UniquePtr<Window> Window::create(const WindowDesc& desc)
{
#ifdef _WIN32
	return std::make_unique<Win32Window>(desc);
#else
#error "Window creation is currently only supported on Windows."
#endif
}