#ifndef GENESIS_MESH_RENDERER_COMPONENT_H
#define GENESIS_MESH_RENDERER_COMPONENT_H
#include <entity/components/Component.h>

GENESIS_NAMESPACE_BEGIN

class MeshRendererComponent final: public Component
{
public:
	explicit MeshRendererComponent(const ComponentDesc& desc);
	~MeshRendererComponent() override;

	const Mesh* getMesh() const;
	const Material* getMaterial() const;

	void setMesh(const SharedPtr<Mesh>& mesh);
	void setMaterial(const SharedPtr<Material>& material);

private:
	SharedPtr<Mesh> m_mesh;
	SharedPtr<Material> m_material;
};

GENESIS_NAMESPACE_END

#endif