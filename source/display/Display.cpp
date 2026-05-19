#include <display/Display.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/SwapChain.h>

using namespace genesis;

Display::Display(const DisplayDesc& desc): Base(desc.window.base), m_borderless{false}, m_matchWindowResolution{true}, m_onResizeWindow{nullptr}
{
	m_window = Window::create(desc.window);
	m_swapChain = desc.graphicsContext.graphicsDevice.createSwapChain({m_window->getHandle(), m_window->getSize()});
	
	m_window->onResize([&](uint32 width, uint32 height) {
		if (m_matchWindowResolution) {
			m_swapChain->resize(width, height);
		}
		if (m_onResizeWindow) {
			m_onResizeWindow(width, height);
		}
	});
}

Display::~Display() {}

Rect Display::getWindowSize() const noexcept
{
	return m_window->getSize();
}

Rect Display::getImageResolution() const noexcept
{
	return m_swapChain->getSize();
}

bool Display::isBorderless() const noexcept
{
	return m_borderless;
}

Window& Display::getWindow() noexcept
{
	return *m_window;
}

SwapChain& Display::getSwapChain() noexcept
{
	return *m_swapChain;
}

void Display::resizeWindow(uint32 width, uint32 height)
{
	m_window->resize(width, height);
}

void Display::toggleBorderless(uint32 width, uint32 height)
{
	m_borderless = !m_borderless;
	if (m_borderless) {
		m_window->setStyle(WindowStyle::Borderless);
		m_window->resize(width, height);
		m_window->setPosition(0, 0);
	}
	else {
		m_window->setStyle(WindowStyle::Windowed);
		m_window->resize(width, height);
		m_window->centerOnScreen();
	}
}

void Display::setMatchWindowResolution(bool matchWindowResolution)
{
	m_matchWindowResolution = matchWindowResolution;
}

void Display::onResizeWindow(std::function<void(uint32, uint32)> callback)
{
	m_onResizeWindow = callback;
}