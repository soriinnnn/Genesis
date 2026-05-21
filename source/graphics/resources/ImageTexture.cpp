#include <graphics/resources/ImageTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <DirectXTex.h>

using namespace genesis;
using namespace std;

static D3D11_TEXTURE2D_DESC getTextureDesc(const ImageTextureSolidDesc& desc);
static D3D11_SHADER_RESOURCE_VIEW_DESC getShaderResourceViewDesc(const DirectX::ScratchImage& imageData);

ImageTexture::ImageTexture(const ImageTextureFileDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	DirectX::ScratchImage imageData{};

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		DirectX::LoadFromWICFile(
			tdesc.path,
			DirectX::WIC_FLAGS_NONE,
			nullptr,
			imageData
		),
		"LoadFromWICFile failed."
	);

	DirectX::ScratchImage mipMappedImage;
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		DirectX::GenerateMipMaps(
			imageData.GetImages(),
			imageData.GetImageCount(),
			imageData.GetMetadata(),
			DirectX::TEX_FILTER_DEFAULT,
			0,
			mipMappedImage
		),
		"GenerateMipMaps failed."
	);

	Microsoft::WRL::ComPtr<ID3D11Resource> texture;
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		DirectX::CreateTexture(
			&m_device,
			mipMappedImage.GetImages(),
			mipMappedImage.GetImageCount(),
			mipMappedImage.GetMetadata(),
			&texture
		),
		"CreateTexture failed."
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = getShaderResourceViewDesc(mipMappedImage);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(
			texture.Get(),
			&resourceViewDesc,
			&m_resourceView
		),
		"CreateShaderResourceView failed."
	);

	m_size = Rect{static_cast<int32>(imageData.GetMetadata().width), static_cast<int32>(imageData.GetMetadata().height)};
}

ImageTexture::ImageTexture(const ImageTextureSolidDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_size{tdesc.size}
{
	uint8 rgba[4] = {};
	rgba[0] = static_cast<uint8>(tdesc.color.x * 255);
	rgba[1] = static_cast<uint8>(tdesc.color.y * 255);
	rgba[2] = static_cast<uint8>(tdesc.color.z * 255);
	rgba[3] = static_cast<uint8>(tdesc.color.w * 255);

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = rgba;
	initData.SysMemPitch = sizeof(uint32);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	D3D11_TEXTURE2D_DESC textureDesc = getTextureDesc(tdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateTexture2D(
			&textureDesc, 
			&initData, 
			&texture
		),
		"CreateTexture2D failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(
			texture.Get(),
			nullptr,
			&m_resourceView
		),
		"CreateShaderResourceView failed."
	);
}

ImageTexture::~ImageTexture() {}

Rect ImageTexture::getSize() const noexcept
{
	return m_size;
}

/* STATIC FUNCTION DEFINITIONS */

D3D11_SHADER_RESOURCE_VIEW_DESC getShaderResourceViewDesc(const DirectX::ScratchImage& imageData)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

	desc.Format = imageData.GetMetadata().format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = static_cast<UINT>(imageData.GetMetadata().mipLevels);
	desc.Texture2D.MostDetailedMip = 0;

	return desc;
}

D3D11_TEXTURE2D_DESC getTextureDesc(const ImageTextureSolidDesc& desc)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};

	textureDesc.Width = static_cast<UINT>(desc.size.width());
	textureDesc.Height = static_cast<UINT>(desc.size.height());
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	return textureDesc;
}