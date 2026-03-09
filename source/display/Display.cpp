#include <display/Display.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/SwapChain.h>

using namespace genesis;

Display::Display(const DisplayDesc& desc): Base(desc.window.base)
{
	m_window = Window::create(desc.window);
	m_swapChain = desc.graphicsDevice.createSwapChain({m_window->getHandle(), m_window->getSize()});

	m_window->onResize([this](uint32 width, uint32 height) {
		m_swapChain->resize(width, height);
	});
}

Display::~Display() {}

Rect Display::getSize() const noexcept
{
	return m_window->getSize();
}

Window& Display::getWindow() noexcept
{
	return *m_window;
}

SwapChain& Display::getSwapChain() noexcept
{
	return *m_swapChain;
}

void Display::resize(uint32 width, uint32 height)
{
	m_window->resize(width, height);
}