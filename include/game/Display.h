#ifndef GENESIS_DISPLAY_H
#define GENESIS_DISPLAY_H
#include <window/Window.h>

namespace genesis 
{
	class Display final: public Window
	{
	public:
		Display(const DisplayDesc& desc);
		~Display() override;

		SwapChain& getSwapChain() noexcept;

	private:
		SwapChainPtr m_swapChain;
	};
}

#endif