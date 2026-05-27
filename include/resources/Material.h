#ifndef GENESIS_MATERIAL_H
#define GENESIS_MATERIAL_H
#include <resources/Resource.h>

GENESIS_NAMESPACE_BEGIN

struct MaterialDesc
{
	ResourceDesc resource;
};

class Material final: public Resource
{
public:
	struct TextureBinding
	{
		SharedPtr<Texture> texture;
		uint32 slot;
	};

	struct SamplerBinding
	{
		SharedPtr<SamplerState> sampler;
		uint32 slot;
	};

public:
	explicit Material(const MaterialDesc& mdesc);
	~Material() override;

	bool hasProperties() const noexcept;
	const Vector<TextureBinding>& getTextures() const noexcept;
	const Vector<SamplerBinding>& getSamplers() const noexcept;
	const GraphicsPipelineState& getGraphicsPipelineState() const noexcept;
	const ConstantBuffer& getProperties() const;

private:
	Vector<TextureBinding> m_textures;
	Vector<SamplerBinding> m_samplers;
	SharedPtr<GraphicsPipelineState> m_pipeline;
	SharedPtr<ConstantBuffer> m_properties;
};

GENESIS_NAMESPACE_END

#endif