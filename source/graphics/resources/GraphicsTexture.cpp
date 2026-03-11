#include <graphics/resources/GraphicsTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <DirectXTex.h>

using namespace genesis;
using namespace std;
using namespace DirectX;

static D3D11_SHADER_RESOURCE_VIEW_DESC getShaderResourceViewDesc(const ScratchImage& imageData);

GraphicsTexture::GraphicsTexture(const GraphicsTextureDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	ScratchImage imageData{};

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		LoadFromWICFile(
			tdesc.path,
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

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = getShaderResourceViewDesc(imageData);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(
			m_texture.Get(), 
			&resourceViewDesc, 
			&m_resourceView
		),
		"CreateShaderResourceView failed."
	);
}

GraphicsTexture::~GraphicsTexture() {}

static D3D11_SHADER_RESOURCE_VIEW_DESC getShaderResourceViewDesc(const ScratchImage& imageData)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

	desc.Format = imageData.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = static_cast<UINT>(imageData.GetMetadata().mipLevels);
	desc.Texture2D.MostDetailedMip = 0;

	return desc;
};