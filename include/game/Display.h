#ifndef GENESIS_DISPLAY_H
#define GENESIS_DISPLAY_H
#include <window/Window.h>

namespace genesis 
{
	class Display final: public Window
	{
	public:
		explicit Display(const DisplayDesc& desc);
		~Display() override;

		SwapChain& getSwapChain() noexcept;

	protected:
		void onResize() override;

	private:
		SwapChainPtr m_swapChain;
	};
}

#endif