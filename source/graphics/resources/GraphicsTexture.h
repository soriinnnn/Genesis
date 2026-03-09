#ifndef GENESIS_GRAPHICS_TEXTURE_H
#define GENESIS_GRAPHICS_TEXTURE_H
#include <graphics/resources/GraphicsResource.h>

namespace genesis
{
	struct GraphicsTextureDesc
	{
		const wchar_t* path;
	};

	class GraphicsTexture final: public GraphicsResource
	{
	public:
		GraphicsTexture(const GraphicsTextureDesc& gDesc, const GraphicsResourceDesc& grDesc);
		~GraphicsTexture() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	};
}

#endif