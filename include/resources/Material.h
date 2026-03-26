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

		Shader& getVertexShader() noexcept;
		Shader& getPixelShader() noexcept;
		GraphicsPipelineState& getGraphicsPipelineState() noexcept;
		ConstantBuffer& getProperties();

	private:
		SharedPtr<Shader> m_vertexShader;
		SharedPtr<Shader> m_pixelShader;
		std::vector<SharedPtr<Texture>> m_textures;
		SharedPtr<GraphicsPipelineState> m_pipeline;
		SharedPtr<ConstantBuffer> m_properties;
		uint32 m_propertiesSlot;
	};
}

#endif