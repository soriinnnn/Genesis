#include <graphics/resources/DepthStencilTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_TEXTURE2D_DESC getTextureDesc(const DepthStencilTextureDesc& desc);

DepthStencilTexture::DepthStencilTexture(const DepthStencilTextureDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	D3D11_TEXTURE2D_DESC desc = getTextureDesc(tdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateTexture2D(
			&desc,
			nullptr,
			&m_texture
		),
		"CreateTexture2D failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateDepthStencilView(
			m_texture.Get(),
			nullptr,
			&m_depthStencilView
		),
		"CreateDepthStencilView failed."
	);
}

DepthStencilTexture::~DepthStencilTexture() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_TEXTURE2D_DESC getTextureDesc(const DepthStencilTextureDesc& desc)
{
	D3D11_TEXTURE2D_DESC textureDesc{};

	textureDesc.Width = static_cast<UINT>(desc.size.width());
	textureDesc.Height = static_cast<UINT>(desc.size.height());
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	return textureDesc;
}