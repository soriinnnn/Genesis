#ifndef GENESIS_MESH_RENDERER_H
#define GENESIS_MESH_RENDERER_H
#include <entity/components/Component.h>

namespace genesis
{
	class MeshRenderer final: public Component
	{
	public:
		explicit MeshRenderer(const ComponentDesc& desc);
		~MeshRenderer() override;

		Mesh* getMesh();
		Texture* getTexture();
		GraphicsPipelineState* getGraphicsPipelineState();

		void setMesh(const SharedPtr<Mesh>& mesh);
		void setTexture(const SharedPtr<Texture>& texture);
		void setGraphicsPipelineState(const SharedPtr<GraphicsPipelineState>& pipeline);

	private:
		SharedPtr<Mesh> m_mesh;
		SharedPtr<Texture> m_texture;
		SharedPtr<GraphicsPipelineState> m_pipeline;
	};
}

#endif