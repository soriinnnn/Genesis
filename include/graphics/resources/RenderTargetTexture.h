#ifndef GENESIS_RENDER_TARGET_TEXTURE_H
#define GENESIS_RENDER_TARGET_TEXTURE_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>

namespace genesis
{
	struct RenderTargetTextureDesc
	{
		Rect size;
		uint32 sampleCount = 1;
	};

	class RenderTargetTexture final: public GraphicsResource
	{
	public:
		RenderTargetTexture(const RenderTargetTextureDesc& tdesc, const GraphicsResourceDesc& gdesc);
		~RenderTargetTexture() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resourceView;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_targetView;
		DXGI_FORMAT m_format;
		uint32 m_sampleCount;
		Rect m_size;

		friend class DeviceContext;
	};
}

#endif

