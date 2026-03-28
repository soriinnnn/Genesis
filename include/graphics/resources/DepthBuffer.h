#ifndef GENESIS_DEPTH_BUFFER_H
#define GENESIS_DEPTH_BUFFER_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>

namespace genesis
{
	struct DepthBufferDesc
	{
		Rect size;
	};

	class DepthBuffer final: GraphicsResource
	{
	public:
		DepthBuffer(const DepthBufferDesc& bdesc, const GraphicsResourceDesc& gdesc);
		~DepthBuffer() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthView;

		friend class DeviceContext;
	};
}

#endif