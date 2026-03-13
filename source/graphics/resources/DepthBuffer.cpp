#include <graphics/resources/DepthBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_TEXTURE2D_DESC getDepthTextureDesc(const DepthBufferDesc& desc);

DepthBuffer::DepthBuffer(const DepthBufferDesc& bdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	D3D11_TEXTURE2D_DESC buffDesc = getDepthTextureDesc(bdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateTexture2D(
			&buffDesc,
			nullptr,
			&m_depthTexture
		),
		"CreateTexture2D failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateDepthStencilView(
			m_depthTexture.Get(),
			nullptr, 
			&m_depthView
		),
		"CreateDepthStencilView failed."
	);
}

DepthBuffer::~DepthBuffer() {}

/* STATIC FUNCTION DEFINITIONS */

static D3D11_TEXTURE2D_DESC getDepthTextureDesc(const DepthBufferDesc& desc)
{
	D3D11_TEXTURE2D_DESC depthDesc{};

	depthDesc.Width = static_cast<UINT>(desc.size.width());
	depthDesc.Height = static_cast<UINT>(desc.size.height());
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	return depthDesc;
}