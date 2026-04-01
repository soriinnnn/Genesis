#include <graphics/resources/RenderTargetTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_TEXTURE2D_DESC getTextureDesc(const RenderTargetTextureDesc& desc);

RenderTargetTexture::RenderTargetTexture(const RenderTargetTextureDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	D3D11_TEXTURE2D_DESC textureDesc = getTextureDesc(tdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateTexture2D(
			&textureDesc,
			nullptr,
			&m_texture
		),
		"CreateTexture2D failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(
			m_texture.Get(),
			nullptr,
			&m_resourceView
		),
		"CreateShaderResourceView failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateRenderTargetView(
			m_texture.Get(),
			nullptr,
			&m_renderTargetView
		),
		"CreateRenderTargetView failed."
	);
}

RenderTargetTexture::~RenderTargetTexture() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_TEXTURE2D_DESC getTextureDesc(const RenderTargetTextureDesc& desc)
{
	D3D11_TEXTURE2D_DESC textureDesc{};

	textureDesc.Width = static_cast<UINT>(desc.size.width());
	textureDesc.Height = static_cast<UINT>(desc.size.height());
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	return textureDesc;
}