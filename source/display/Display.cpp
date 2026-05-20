#include <display/Display.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/SwapChain.h>

using namespace genesis;
using namespace std;

Display::Display(const DisplayDesc& desc): Base(desc.base), m_window{desc.window}
{
	m_swapChain = desc.graphicsContext.graphicsDevice.createSwapChain({m_window.getHandle(), m_window.getSize()});
	m_window.onResize([this](Rect size) {
		if (!isBorderless()) {
			m_windowedSize = size;
		}
		if (m_matchWindowResolution) {
			m_swapChain->setSize(size);
		}
		if (m_onResize) {
			m_onResize(size);
		}
	});
	m_windowedSize = m_window.getSize();
	m_vsync = true;
	m_isBorderless = false;
	m_matchWindowResolution = true;
	m_onResize = nullptr;
}

Display::~Display() 
{
	m_window.onResize(nullptr);
}

SwapChain& Display::getSwapChain() noexcept
{
	return *m_swapChain;
}

bool Display::isBorderless() const noexcept
{
	return m_isBorderless;
}

bool Display::getVSync() const noexcept
{
	return m_vsync;
}

Rect Display::getSize() const noexcept
{
	return m_window.getSize();
}

Rect Display::getResolution() const noexcept
{
	return m_swapChain->getSize();
}

void Display::setSize(const Rect& size)
{
	if (isBorderless()) {
		GENESIS_LOG_WARNING("Cannot change window size while in borderless mode.");
		return;
	}
	m_window.setSize(size);
}

void Display::setResolution(const Rect& resolution)
{
	m_swapChain->setSize(resolution);
}

void Display::setBorderless(bool enable)
{
	if (m_isBorderless == enable) {
		return;
	}
	m_isBorderless = enable;
	if (enable) {
		m_window.setSize(m_window.getScreenSize());
		m_window.setStyle(WindowStyle::Borderless);
		m_window.setPosition(Point{0, 0});
	}
	else {
		m_window.setSize(m_windowedSize);
		m_window.setStyle(WindowStyle::Windowed);
		m_window.center();
	}
}

void Display::setVSync(bool enable)
{
	m_vsync = enable;
}

void Display::setMatchWindowResolution(bool enable)
{
	if (m_matchWindowResolution == enable) {
		return;
	}
	m_matchWindowResolution = enable;
	if (enable) {
		m_swapChain->setSize(m_windowedSize);
	}
}

void Display::onResize(function<void(Rect)> callback)
{
	m_onResize = move(callback);
}