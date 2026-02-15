#ifndef GENESIS_SWAP_CHAIN_H
#define GENESIS_SWAP_CHAIN_H
#include <core/Core.h>
#include <core/Common.h>
#include <graphics/GraphicsResource.h>

namespace genesis
{
	class SwapChain final: public GraphicsResource
	{
	public:
		SwapChain(const SwapChainDesc& scDesc, const GraphicsResourceDesc& grDesc);
		~SwapChain() override;

		Rect getSize() const noexcept;
		void present(bool vsync = false);

	private:
		void updateRenderTargetView();

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTarget;
		Rect m_size;

		friend class DeviceContext;
	};
}

#endif