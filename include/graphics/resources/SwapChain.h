#ifndef GENESIS_SWAPCHAIN_H
#define GENESIS_SWAPCHAIN_H
#include <graphics/resources/GraphicsResource.h>
#include <core/Core.h>
#include <math/Rect.h>

namespace genesis
{
	struct SwapChainDesc
	{
		void* wndHandle{};
		Rect wndSize{};
	};

	class SwapChain final: public GraphicsResource
	{
	public:
		SwapChain(const SwapChainDesc& sdesc, const GraphicsResourceDesc& gdesc);
		~SwapChain() override;

		Rect getSize() const noexcept;
		void resize(uint32 width, uint32 height);
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