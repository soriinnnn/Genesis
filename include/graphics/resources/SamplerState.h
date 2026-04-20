#ifndef GENESIS_SAMPLER_STATE_H
#define GENESIS_SAMPLER_STATE_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
	struct SamplerStateDesc
	{
		TextureFilter filter = TextureFilter::Anisotropic;
		TextureAddressMode addressU = TextureAddressMode::Wrap;
		TextureAddressMode addressV = TextureAddressMode::Wrap;
		TextureAddressMode addressW = TextureAddressMode::Wrap;
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
}

#endif