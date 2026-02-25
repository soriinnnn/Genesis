#include <window/Window.h>
#include <window/win32/Win32Window.h>

using namespace genesis;

Window::Window(const WindowDesc& desc): Base(desc.base)
{
	m_handle = nullptr;
	m_size = desc.size;
	m_onResizeCallback = nullptr;
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

void Window::onResize(std::function<void(uint32, uint32)> callback)
{
	m_onResizeCallback = callback;
}

SharedPtr<Window> Window::create(const WindowDesc& desc)
{
#ifdef _WIN32
	return std::make_shared<Win32Window>(desc);
#else
	GENESIS_LOG_THROW_ERROR("Window creation is only supported on Windows.");
#endif
}