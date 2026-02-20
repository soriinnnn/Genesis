#include <game/Display.h>
#include <graphics/GraphicsDevice.h>

#include <string>

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

void Display::onResize()
{
	std::string str = std::string("Width: ") + std::to_string(m_size.width()) + ", Height: " + std::to_string(m_size.height()) + "\n";
	GENESIS_LOG_INFO(str.c_str());
}
