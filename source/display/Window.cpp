#include <display/Window.h>
#include <display/win32/Win32Window.h>

using namespace genesis;
using namespace std;

Window::Window(const WindowDesc& desc): Base(desc.base)
{
	m_handle = nullptr;
	m_size = desc.size;
	m_style = desc.style;
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

void Window::onResize(function<void(uint32, uint32)> callback)
{
	m_onResize = callback;
}

void Window::onFocusChanged(std::function<void(bool)> callback)
{
	m_onFocusChanged = callback;
}

UniquePtr<Window> Window::create(const WindowDesc& desc)
{
#ifdef _WIN32
	return make_unique<Win32Window>(desc);
#else
#error "Window creation is currently only supported on Windows."
#endif
}