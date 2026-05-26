#ifndef GENESIS_DEPTH_STENCIL_TEXTURE_H
#define GENESIS_DEPTH_STENCIL_TEXTURE_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>

namespace genesis
{
	struct DepthStencilTextureDesc
	{
		Rect size;
		uint32 sampleCount = 1;
	};

	class DepthStencilTexture final: public GraphicsResource
	{
	public:
		DepthStencilTexture(const DepthStencilTextureDesc& tdesc, const GraphicsResourceDesc& gdesc);
		~DepthStencilTexture() override;

		Rect getSize() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		DXGI_FORMAT m_format;
		Rect m_size;

		friend class DeviceContext;
	};
}

#endif