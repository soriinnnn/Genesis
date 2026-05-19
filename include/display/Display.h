#ifndef GENESIS_DISPLAY_H
#define GENESIS_DISPLAY_H
#include <display/Window.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis 
{
	struct DisplayDesc
	{
		WindowDesc window;
		GraphicsContext graphicsContext;
	};

	class Display final: public Base
	{
	public:
		explicit Display(const DisplayDesc& desc);
		~Display() override;

		Window& getWindow() noexcept;
		SwapChain& getSwapChain() noexcept;

		Rect getWindowSize() const noexcept;
		Rect getImageResolution() const noexcept;
		bool isBorderless() const noexcept;

		void resizeWindow(uint32 width, uint32 height);
		void toggleBorderless(uint32 width, uint32 height);
		void setMatchWindowResolution(bool matchWindowResolution);

		void onResizeWindow(std::function<void(uint32, uint32)> callback);

	private:
		UniquePtr<Window> m_window;
		SharedPtr<SwapChain> m_swapChain;
		bool m_borderless;
		bool m_matchWindowResolution;

	private:
		std::function<void(uint32, uint32)> m_onResizeWindow;
	};
}

#endif