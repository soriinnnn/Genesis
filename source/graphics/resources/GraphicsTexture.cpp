#include <graphics/resources/GraphicsTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <DirectXTex.h>

using namespace genesis;
using namespace std;
using namespace DirectX;

GraphicsTexture::GraphicsTexture(const GraphicsTextureDesc& gDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	ScratchImage imageData{};

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		LoadFromWICFile(
			gDesc.path,
			WIC_FLAGS_NONE,
			nullptr,
			imageData
		),
		"LoadFromWICFile failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		CreateTexture(
			&m_device,
			imageData.GetImages(),
			imageData.GetImageCount(),
			imageData.GetMetadata(),
			&m_texture
		),
		"CreateTexture failed."
	);
}

GraphicsTexture::~GraphicsTexture() {}
