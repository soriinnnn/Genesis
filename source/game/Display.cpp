#include <game/Display.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;

// --------------------------------------------------------------------------------

Display::Display(const DisplayDesc& desc): Window(desc.window)
{
	m_swapChain = desc.graphicsDevice.createSwapChain({m_handle, m_size});
}

Display::~Display() {}

SwapChain& Display::getSwapChain() noexcept
{
	return *m_swapChain;
}
