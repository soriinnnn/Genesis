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

		SwapChain& getSwapChain() noexcept;
		void resize(uint32 width, uint32 height);

	private:
		WindowPtr m_window;
		SwapChainPtr m_swapChain;
	};
}

#endif