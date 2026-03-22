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
		Material* getMaterial();

		void setMesh(const SharedPtr<Mesh>& mesh);
		void setMaterial(const SharedPtr<Material>& material);

	private:
		SharedPtr<Mesh> m_mesh;
		SharedPtr<Material> m_material;
	};
}

#endif