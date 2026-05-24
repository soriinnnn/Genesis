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
		Rect size{};
	};

	class SwapChain final: public GraphicsResource
	{
	public:
		SwapChain(const SwapChainDesc& sdesc, const GraphicsResourceDesc& gdesc);
		~SwapChain() override;

		bool isFullscreen() const;
		Rect getSize() const noexcept;

		bool setFullscreen(bool enable);
		void setSize(const Rect& size);
		void present(bool vsync = false);

	private:
		void updateBuffers();

	private:
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_targetView;
		SharedPtr<DepthStencilTexture> m_depthStencil;
		Rect m_size;

		friend class DeviceContext;
	};
}

#endif