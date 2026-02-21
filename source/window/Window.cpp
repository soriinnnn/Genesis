#include <window/Window.h>

using namespace genesis;

Window::Window(const WindowDesc& desc): Base(desc.base)
{
	m_handle = nullptr;
	m_size = desc.size;
}

Window::~Window() {}