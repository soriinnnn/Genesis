#include <graphics/resources/SamplerState.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_SAMPLER_DESC getD3D11SamplerDesc(TextureFilter filter, TextureAddressMode addressU, TextureAddressMode addressV, TextureAddressMode addressW, ComparisonFunction comparison, uint32 maxAnisotropy, float minLOD, float maxLOD);

SamplerState::SamplerState(const SamplerStateDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (sdesc.maxAnisotropy <= 0 || sdesc.maxAnisotropy > D3D11_REQ_MAXANISOTROPY) {
		GENESIS_LOG_THROW_INVALID_ARG("maxAnisotropy ({}) must be between 1 and {}.", sdesc.maxAnisotropy, D3D11_REQ_MAXANISOTROPY);
	}
	if (sdesc.minLOD > sdesc.maxLOD) {
		GENESIS_LOG_THROW_INVALID_ARG("minLOD ({}) must be less than or equal to maxLOD ({}).", sdesc.minLOD, sdesc.maxLOD);
	}

	D3D11_SAMPLER_DESC desc = getD3D11SamplerDesc(
		sdesc.filter, 
		sdesc.addressU,
		sdesc.addressV,
		sdesc.addressW,
		sdesc.comparison,
		sdesc.maxAnisotropy,
		sdesc.minLOD,
		sdesc.maxLOD
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateSamplerState(&desc, &m_samplerState),
		"CreateSamplerState failed."
	);
}

SamplerState::~SamplerState() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_SAMPLER_DESC getD3D11SamplerDesc(TextureFilter filter, TextureAddressMode addressU, TextureAddressMode addressV, TextureAddressMode addressW, ComparisonFunction comparison, uint32 maxAnisotropy, float minLOD, float maxLOD)
{
	D3D11_SAMPLER_DESC desc{};

	desc.Filter = graphicsUtils::getD3D11Filter(filter);
	desc.AddressU = graphicsUtils::getD3D11TextureAddressMode(addressU);
	desc.AddressV = graphicsUtils::getD3D11TextureAddressMode(addressV);
	desc.AddressW = graphicsUtils::getD3D11TextureAddressMode(addressW);
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = static_cast<UINT>(maxAnisotropy);
	desc.ComparisonFunc = graphicsUtils::getD3D11ComparisonFunc(comparison);
	desc.BorderColor[0] = 0.0f;
	desc.BorderColor[1] = 0.0f;
	desc.BorderColor[2] = 0.0f;
	desc.BorderColor[3] = 0.0f;
	desc.MinLOD = minLOD;
	desc.MaxLOD = maxLOD;

	return desc;
}