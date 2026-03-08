#ifndef GENESIS_DISPLAY_H
#define GENESIS_DISPLAY_H
#include <window/Window.h>

namespace genesis 
{
	struct DisplayDesc
	{
		WindowDesc window;
		GraphicsDevice& graphicsDevice;
	};

	class Display final: public Base
	{
	public:
		explicit Display(const DisplayDesc& desc);
		~Display() override;

		Rect getSize() const noexcept;

		Window& getWindow() noexcept;
		SwapChain& getSwapChain() noexcept;
		void resize(uint32 width, uint32 height);

	private:
		SharedPtr<Window> m_window;
		SharedPtr<SwapChain> m_swapChain;
	};
}

#endif