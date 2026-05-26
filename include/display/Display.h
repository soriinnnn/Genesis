#ifndef GENESIS_DISPLAY_H
#define GENESIS_DISPLAY_H
#include <display/Window.h>
#include <core/utils/Types.h>

namespace genesis 
{
	enum class DisplayMode
	{
		Windowed,
		Borderless,
		Fullscreen
	};

	struct DisplayDesc
	{
		BaseDesc base;
		Window& window;
		GraphicsContext graphicsContext;
	};

	class Display final: public Base
	{
	public:
		explicit Display(const DisplayDesc& desc);
		~Display() override;

		const SwapChain& getSwapChain() noexcept;

		Rect getSize() const noexcept;
		Rect getResolution() const noexcept;
		DisplayMode getMode() const noexcept;
		bool getVSync() const noexcept;

		void setSize(const Rect& size);
		void setResolution(const Rect& resolution);
		void setMode(DisplayMode mode);
		void setVSync(bool enable);
		void setSyncResolution(bool enable);
		void present();

		void onResize(std::function<void(Rect)> callback);

	private:
		Window& m_window;
		SharedPtr<SwapChain> m_swapChain;
		DisplayMode m_mode;
		bool m_isTransitioning;
		Rect m_screenSize;
		Rect m_windowedSize;
		bool m_vsync;
		bool m_syncResolution;

	private:
		std::function<void(Rect)> m_onResize;
	};
}

#endif