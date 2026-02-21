#ifndef GENESIS_DISPLAY_H
#define GENESIS_DISPLAY_H
#include <window/Window.h>

namespace genesis 
{
	class Display final: public Base
	{
	public:
		explicit Display(const DisplayDesc& desc);
		~Display() override;

		SwapChain& getSwapChain() noexcept;

	private:
		WindowPtr m_window;
		SwapChainPtr m_swapChain;
	};
}

#endif