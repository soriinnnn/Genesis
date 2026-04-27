#ifndef GENESIS_MESH_RENDERER_COMPONENT_H
#define GENESIS_MESH_RENDERER_COMPONENT_H
#include <entity/components/Component.h>

namespace genesis
{
	class MeshRendererComponent final: public Component
	{
	public:
		explicit MeshRendererComponent(const ComponentDesc& desc);
		~MeshRendererComponent() override;

		Mesh* getMesh();
		Material* getMaterial();

		void setMesh(const SharedPtr<Mesh>& mesh);
		void setMaterial(const SharedPtr<Material>& material);

	private:
		SharedPtr<Mesh> m_mesh;
		SharedPtr<Material> m_material;
	};
}

#endif