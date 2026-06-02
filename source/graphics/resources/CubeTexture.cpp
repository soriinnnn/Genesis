#include <graphics/resources/CubeTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <DirectXTex.h>

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

static D3D11_TEXTURE2D_DESC getD3D11TextureDesc(const Rect& size);
static D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer, int32 rowPitch, int32 slicePitch);
static D3D11_SHADER_RESOURCE_VIEW_DESC getD3D11ShaderResourceViewDesc();

CubeTexture::CubeTexture(const CubeTextureDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	DirectX::ScratchImage images[6];
	DirectX::TexMetadata firstMetadata{};

	for (int i = 0; i < 6; i++) {
		DirectX::TexMetadata metadata{};
		WString path{tdesc.cubeFaces[i], tdesc.cubeFaces[i] + strlen(tdesc.cubeFaces[i])};

		GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
			DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, images[i]),
			"LoadFromWICFile failed."
		);

		if (i == 0) {
			firstMetadata = metadata;
		}
		else if (firstMetadata.width != metadata.width || firstMetadata.height != metadata.height) {
			GENESIS_LOG_THROW_ERROR("Cube texture faces have mismatched dimensions.");
		}
	}
	THROW_ON_INVALID_TEXTURE_SIZE(firstMetadata.width, firstMetadata.height);
	m_size = Rect{static_cast<int32>(firstMetadata.width), static_cast<int32>(firstMetadata.height)};

	D3D11_SUBRESOURCE_DATA initData[6];
	for (int i = 0; i < 6; i++) {
		const DirectX::Image* image = images[i].GetImage(0, 0, 0);
		initData[i] = getD3D11SubResourceData(
			image->pixels, 
			static_cast<int32>(image->rowPitch), 
			static_cast<int32>(image->slicePitch)
		);
	}

	D3D11_TEXTURE2D_DESC textureDesc = getD3D11TextureDesc(m_size);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateTexture2D(&textureDesc, initData, &m_texture),
		"CreateTexture2D failed."
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = getD3D11ShaderResourceViewDesc();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(m_texture.Get(), &resourceViewDesc, &m_resourceView),
		"CreateShaderResourceView failed."
	);
}

CubeTexture::~CubeTexture() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_TEXTURE2D_DESC getD3D11TextureDesc(const Rect& size)
{
	D3D11_TEXTURE2D_DESC desc{};

	desc.Width = static_cast<UINT>(size.width());
	desc.Height = static_cast<UINT>(size.height());
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	return desc;
}

D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer, int32 rowPitch, int32 slicePitch)
{
	D3D11_SUBRESOURCE_DATA data{};

	data.pSysMem = buffer;
	data.SysMemPitch = rowPitch;
	data.SysMemSlicePitch = slicePitch;

	return data;
}

D3D11_SHADER_RESOURCE_VIEW_DESC getD3D11ShaderResourceViewDesc()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	desc.TextureCube.MostDetailedMip = 0;
	desc.TextureCube.MipLevels = 1;

	return desc;
}