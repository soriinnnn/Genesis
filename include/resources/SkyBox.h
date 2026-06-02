#ifndef GENESIS_SKY_BOX_H
#define GENESIS_SKY_BOX_H
#include <core/Core.h>
#include <resources/Resource.h>

GENESIS_NAMESPACE_BEGIN

struct SkyBoxDesc
{
	ResourceDesc resource;
};

class SkyBox final: public Resource
{
public:
	explicit SkyBox(const SkyBoxDesc& desc);
	~SkyBox() override;

	const CubeTexture& getCubeTexture() const;
	const SamplerState& getSamplerState() const;

private:
	SharedPtr<CubeTexture> m_texture;
	SharedPtr<SamplerState> m_sampler;
};

GENESIS_NAMESPACE_END

#endif