#include <graphics/resources/DepthStencilTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_TEXTURE2D_DESC getD3D11TextureDesc(const Rect& size, DXGI_FORMAT format, uint32 sampleCount, uint32 sampleQualityLevel);

DepthStencilTexture::DepthStencilTexture(const DepthStencilTextureDesc& tdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_size{tdesc.size}, m_format{DXGI_FORMAT_D24_UNORM_S8_UINT}
{
	if (m_size.width() <= 0 || m_size.width() > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) {
		GENESIS_LOG_THROW_INVALID_ARG("Texture width {} out of valid range (1..{}).", m_size.width(), D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION);
	}
	if (m_size.height() <= 0 || m_size.height() > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION) {
		GENESIS_LOG_THROW_INVALID_ARG("Texture height {} out of valid range (1..{}).", m_size.height(), D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION);
	}

	uint32 sampleQualityLevel;
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CheckMultisampleQualityLevels(
			m_format,
			tdesc.sampleCount,
			&sampleQualityLevel
		),
		"CheckMultisampleQualityLevels failed."
	);

	D3D11_TEXTURE2D_DESC textureDesc = getD3D11TextureDesc(m_size, m_format, tdesc.sampleCount, sampleQualityLevel);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateTexture2D(
			&textureDesc,
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

Rect DepthStencilTexture::getSize() const noexcept
{
	return m_size;
}

/* STATIC FUNCTION DEFINITIONS */

D3D11_TEXTURE2D_DESC getD3D11TextureDesc(const Rect& size, DXGI_FORMAT format, uint32 sampleCount, uint32 sampleQualityLevel)
{
	D3D11_TEXTURE2D_DESC desc{};

	desc.Width = static_cast<UINT>(size.width());
	desc.Height = static_cast<UINT>(size.height());
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = static_cast<UINT>(sampleCount);
	desc.SampleDesc.Quality = (sampleQualityLevel > 0) ? static_cast<UINT>(sampleQualityLevel - 1) : 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	return desc;
}