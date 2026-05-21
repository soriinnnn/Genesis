#include <display/Window.h>
#include <display/win32/Win32Window.h>

using namespace genesis;
using namespace std;

Window::Window(const WindowDesc& desc): Base(desc.base)
{
	m_handle = nullptr;
	m_position = Point{0, 0};
	m_size = desc.size;
	m_style = desc.style;
	m_title = String{desc.title};
	m_isVisible = false;
	m_hasFocus = false;
}

Window::~Window() {}

const void* Window::getHandle() const noexcept
{
	return m_handle;
}

void* Window::getHandle() noexcept
{
	return m_handle;
}

Point Window::getPosition() const noexcept
{
	return m_position;
}

Rect Window::getSize() const noexcept
{
	return m_size;
}

WindowStyle Window::getStyle() const noexcept
{
	return m_style;
}

const char* Window::getTitle() const noexcept
{
	return m_title.c_str();
}

bool Window::isVisible() const noexcept
{
	return m_isVisible;
}

bool Window::hasFocus() const noexcept
{
	return m_hasFocus;
}

void Window::onResize(function<void(Rect)> callback)
{
	m_onResize = move(callback);
}

void Window::onFocusChanged(function<void(bool)> callback)
{
	m_onFocusChanged = move(callback);
}

UniquePtr<Window> Window::create(const WindowDesc& desc)
{
#ifdef _WIN32
	return make_unique<Win32Window>(desc);
#else
#error "Window creation is currently only supported on Windows."
#endif
}