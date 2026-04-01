#ifndef GENESIS_IMAGE_TEXTURE_H
#define GENESIS_IMAGE_TEXTURE_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>

namespace genesis
{
	struct ImageTextureDesc
	{
		const wchar_t* path{};
	};

	class ImageTexture final: public GraphicsResource
	{
	public:
		ImageTexture(const ImageTextureDesc& tdesc, const GraphicsResourceDesc& gdesc);
		~ImageTexture() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resourceView;

		friend class DeviceContext;
	};
}

#endif
