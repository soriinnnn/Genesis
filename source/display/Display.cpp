#include <display/Display.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/SwapChain.h>

using namespace genesis;
using namespace std;

Display::Display(const DisplayDesc& desc): Base(desc.base), m_window{desc.window}
{
	m_swapChain = desc.graphicsContext.graphicsDevice.createSwapChain({m_window.getHandle(), m_window.getSize()});
	m_mode = DisplayMode::Windowed;
	m_isTransitioning = false;
	m_screenSize = m_window.getScreenSize();
	m_windowedSize = m_window.getSize();
	m_vsync = true;
	m_syncResolution = true;
	m_onResize = nullptr;
	m_window.onResize([this](Rect size) {
		if (!m_isTransitioning) {
			if (m_mode == DisplayMode::Windowed) {
				m_windowedSize = size;
			}
			else if (m_mode == DisplayMode::Fullscreen) {
				setMode(DisplayMode::Windowed);
			}
		}
		if (m_syncResolution) {
			m_swapChain->setSize(size);
		}
		if (m_onResize) {
			m_onResize(size);
		}
	});
}

Display::~Display() 
{
	m_window.onResize(nullptr);
}

const SwapChain& Display::getSwapChain() noexcept
{
	return *m_swapChain;
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

DisplayMode Display::getMode() const noexcept
{
	return m_mode;
}

void Display::setSize(const Rect& size)
{
	if (m_mode != DisplayMode::Windowed) {
		GENESIS_LOG_WARNING("Cannot set size in current display mode. Only allowed in Windowed mode.");
		return;
	}
	m_window.setSize(size);
}

void Display::setResolution(const Rect& resolution)
{
	m_swapChain->setSize(resolution);
}

void Display::setMode(DisplayMode mode)
{
	if (m_mode == mode) {
		return;
	}

	m_isTransitioning = true;
	if (m_mode == DisplayMode::Fullscreen) {
		if (!m_swapChain->setFullscreen(false)) {
			m_isTransitioning = false;
			return;
		}
	}

	switch (mode) {
		case DisplayMode::Windowed: {
			m_window.setStyle(WindowStyle::Windowed);
			m_window.setSize(m_windowedSize);
			m_window.center();
			break;
		}
		case DisplayMode::Borderless: {
			m_window.setStyle(WindowStyle::Borderless);
			m_window.setSize(m_screenSize);
			m_window.setPosition({0, 0});
			break;
		}
		case DisplayMode::Fullscreen: {
			if (!m_swapChain->setFullscreen(true)) {
				m_isTransitioning = false;
				return;
			};
			break;
		}
		default:
			GENESIS_LOG_THROW_INVALID_ARG("Invalid display mode.");
	}
	m_mode = mode;
	m_isTransitioning = false;
}

void Display::setVSync(bool enable)
{
	m_vsync = enable;
}

void Display::setSyncResolution(bool enable)
{
	if (m_syncResolution == enable) {
		return;
	}
	m_syncResolution = enable;
	if (enable) {
		m_swapChain->setSize(m_windowedSize);
	}
}

void Display::present()
{
	m_swapChain->present(m_vsync);
}

void Display::onResize(function<void(Rect)> callback)
{
	m_onResize = move(callback);
}