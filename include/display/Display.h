#ifndef GENESIS_DISPLAY_H
#define GENESIS_DISPLAY_H
#include <display/Window.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis 
{
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

		SwapChain& getSwapChain() noexcept;

		Rect getSize() const noexcept;
		Rect getResolution() const noexcept;
		bool isBorderless() const noexcept;
		bool getVSync() const noexcept;

		void setSize(const Rect& size);
		void setResolution(const Rect& resolution);
		void setBorderless(bool enable);
		void setVSync(bool enable);
		void setMatchWindowResolution(bool enable);

		void onResize(std::function<void(Rect)> callback);

	private:
		Window& m_window;
		SharedPtr<SwapChain> m_swapChain;
		Rect m_windowedSize;
		bool m_vsync;
		bool m_isBorderless;
		bool m_matchWindowResolution;

	private:
		std::function<void(Rect)> m_onResize;
	};
}

#endif