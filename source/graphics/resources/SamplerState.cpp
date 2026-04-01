#include <graphics/resources/SamplerState.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_SAMPLER_DESC getSamplerDesc(const SamplerStateDesc& desc);

SamplerState::SamplerState(const SamplerStateDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	D3D11_SAMPLER_DESC desc = getSamplerDesc(sdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateSamplerState(&desc, &m_samplerState),
		"CreateSamplerState failed."
	);
}

SamplerState::~SamplerState() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_SAMPLER_DESC getSamplerDesc(const SamplerStateDesc& desc)
{
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = graphicsUtils::getD3D11Filter(desc.filter);
	samplerDesc.AddressU = graphicsUtils::getD3D11TextureAddressMode(desc.addressU);
	samplerDesc.AddressV = graphicsUtils::getD3D11TextureAddressMode(desc.addressV);
	samplerDesc.AddressW = graphicsUtils::getD3D11TextureAddressMode(desc.addressW);
	samplerDesc.ComparisonFunc = graphicsUtils::getD3D11ComparisonFunc(desc.comparison);
	samplerDesc.MaxAnisotropy = desc.maxAnisotropy;
	samplerDesc.MinLOD = desc.minLOD;
	samplerDesc.MaxLOD = desc.maxLOD;

	return samplerDesc;
}