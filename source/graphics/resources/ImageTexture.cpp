#include <graphics/resources/ImageTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <cmath>
#include <DirectXTex.h>

#define RGBA_CHANNEL_COUNT 4
#define THROW_ON_INVALID_TEXTURE_SIZE(width, height)\
{\
	if ((width) <= 0 || (width) > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) {\
		GENESIS_LOG_THROW_INVALID_ARG("Texture width {} out of valid range (1..{}).", (width), D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION);\
	}\
	if ((height) <= 0 || (height) > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) {\
		GENESIS_LOG_THROW_INVALID_ARG("Texture height {} out of valid range (1..{}).", (height), D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION);\
	}\
}

using namespace genesis;
using namespace std;

static D3D11_TEXTURE2D_DESC getD3D11TextureDesc(const Rect& size);
static D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer, int32 rowStride);
static D3D11_SHADER_RESOURCE_VIEW_DESC getD3D11ShaderResourceViewDesc(DXGI_FORMAT format, size_t mipLevels);

ImageTexture::ImageTexture(const ImageTextureFileDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	WString path{tdesc.path, tdesc.path + strlen(tdesc.path)};
	
	DirectX::ScratchImage image{};
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image),
		"LoadFromWICFile failed."
	);
	m_size = Rect{static_cast<int32>(image.GetMetadata().width), static_cast<int32>(image.GetMetadata().height)};
	THROW_ON_INVALID_TEXTURE_SIZE(m_size.width(), m_size.height());

	Microsoft::WRL::ComPtr<ID3D11Resource> texture;
	DirectX::ScratchImage* targetImage = &image;

	DirectX::ScratchImage mipImage;
	if (tdesc.generateMipMaps) {
		GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
			DirectX::GenerateMipMaps(
				image.GetImages(),
				image.GetImageCount(),
				image.GetMetadata(),
				DirectX::TEX_FILTER_DEFAULT,
				0,
				mipImage
			),
			"GenerateMipMaps failed."
		);
		targetImage = &mipImage;
	}

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		DirectX::CreateTexture(
			&m_device,
			targetImage->GetImages(),
			targetImage->GetImageCount(),
			targetImage->GetMetadata(),
			&texture
		),
		"CreateTexture failed."
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = getD3D11ShaderResourceViewDesc(targetImage->GetMetadata().format, targetImage->GetMetadata().mipLevels);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(texture.Get(), &resourceDesc, &m_resourceView),
		"CreateShaderResourceView failed."
	);
}

ImageTexture::ImageTexture(const ImageTextureSolidDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_size{tdesc.size}
{
	THROW_ON_INVALID_TEXTURE_SIZE(m_size.width(), m_size.height());

	int32 size = m_size.width() * m_size.height() * RGBA_CHANNEL_COUNT;
	Vector<uint8> buffer(size);
	for (int i = 0; i < size; i += RGBA_CHANNEL_COUNT) {
		buffer[i]   = static_cast<uint8>(round(tdesc.color.x * 255.0f));
		buffer[i+1] = static_cast<uint8>(round(tdesc.color.y * 255.0f));
		buffer[i+2] = static_cast<uint8>(round(tdesc.color.z * 255.0f));
		buffer[i+3] = static_cast<uint8>(round(tdesc.color.w * 255.0f));
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	D3D11_TEXTURE2D_DESC textureDesc = getD3D11TextureDesc(m_size);
	D3D11_SUBRESOURCE_DATA initData = getD3D11SubResourceData(buffer.data(), m_size.width() * RGBA_CHANNEL_COUNT);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateTexture2D(&textureDesc, &initData, &texture),
		"CreateTexture2D failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(texture.Get(), nullptr, &m_resourceView),
		"CreateShaderResourceView failed."
	);
}

ImageTexture::~ImageTexture() {}

Rect ImageTexture::getSize() const noexcept
{
	return m_size;
}

/* STATIC FUNCTION DEFINITIONS */

D3D11_TEXTURE2D_DESC getD3D11TextureDesc(const Rect& size)
{
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = static_cast<UINT>(size.width());
	desc.Height = static_cast<UINT>(size.height());
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	return desc;
}

D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer, int32 rowStride)
{
	D3D11_SUBRESOURCE_DATA data{};

	data.pSysMem = buffer;
	data.SysMemPitch = static_cast<UINT>(rowStride);
	data.SysMemSlicePitch = 0;

	return data;
}

D3D11_SHADER_RESOURCE_VIEW_DESC getD3D11ShaderResourceViewDesc(DXGI_FORMAT format, size_t mipLevels)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

	desc.Format = format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = 0;
	desc.Texture2D.MipLevels = static_cast<UINT>(mipLevels);

	return desc;
}