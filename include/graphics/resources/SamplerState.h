#ifndef GENESIS_SAMPLER_STATE_H
#define GENESIS_SAMPLER_STATE_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>

GENESIS_NAMESPACE_START

struct SamplerStateDesc
{
	SamplerFilter filter = SamplerFilter::Anisotropic;
	SamplerAddressMode addressU = SamplerAddressMode::Wrap;
	SamplerAddressMode addressV = SamplerAddressMode::Wrap;
	SamplerAddressMode addressW = SamplerAddressMode::Wrap;
	ComparisonFunction comparison = ComparisonFunction::Never;
	uint32 maxAnisotropy = 16;
	float minLOD = 0.0f;
	float maxLOD = D3D11_FLOAT32_MAX;
};

class SamplerState final: public GraphicsResource
{
public:
	SamplerState(const SamplerStateDesc& sdesc, const GraphicsResourceDesc& gdesc);
	~SamplerState() override;

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;

	friend class DeviceContext;
	friend class SpriteBatch;
};

GENESIS_NAMESPACE_END

#endif