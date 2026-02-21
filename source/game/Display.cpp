#include <game/Display.h>
#include <graphics/GraphicsDevice.h>
#include <window/win32/Win32Window.h>

using namespace genesis;

Display::Display(const DisplayDesc& desc): Base(desc.window.base)
{
#ifdef _WIN32
	m_window = std::make_shared<Win32Window>(desc.window);
#else
	GENESIS_LOG_THROW_ERROR("Display creation is only supported on Windows.");
#endif
	m_swapChain = desc.graphicsDevice.createSwapChain({m_window->m_handle, m_window->m_size});
}

Display::~Display() {}

SwapChain& Display::getSwapChain() noexcept
{
	return *m_swapChain;
}