#ifndef GENESIS_MATERIAL_H
#define GENESIS_MATERIAL_H
#include <resources/Resource.h>

namespace genesis
{
	struct MaterialDesc
	{
		ResourceDesc resource;
	};

	class Material final: public Resource
	{
	public:
		explicit Material(const MaterialDesc& mdesc);
		~Material() override;
		
		bool hasProperties() const noexcept;
		uint32 getPropertiesSlot() const;
		const std::vector<SharedPtr<Texture>>& getTextures() const noexcept;

		GraphicsPipelineState& getGraphicsPipelineState() noexcept;
		ConstantBuffer& getProperties();

	private:
		SharedPtr<GraphicsPipelineState> m_pipeline;
		std::vector<SharedPtr<Texture>> m_textures;
		SharedPtr<ConstantBuffer> m_properties;
		uint32 m_propertiesSlot;
	};
}

#endif