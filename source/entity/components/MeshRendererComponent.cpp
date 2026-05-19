#include <entity/components/MeshRendererComponent.h>
#include <entity/components/TransformComponent.h>
#include <graphics/resources/GraphicsPipelineState.h>
#include <resources/Mesh.h>
#include <entity/Entity.h>

using namespace genesis;

MeshRendererComponent::MeshRendererComponent(const ComponentDesc& desc): Component(desc) 
{
	if (!m_entity.hasComponent<TransformComponent>()) {
		m_entity.createComponent<TransformComponent>();
	}
}

MeshRendererComponent::~MeshRendererComponent() {}

const Mesh* MeshRendererComponent::getMesh() const
{
	GENESIS_ASSERT(m_mesh.get() != nullptr, "Mesh is null.");
	return m_mesh.get();
}

const Material* MeshRendererComponent::getMaterial() const
{
	GENESIS_ASSERT(m_material.get() != nullptr, "Material is null.");
	return m_material.get();
}

void MeshRendererComponent::setMesh(const SharedPtr<Mesh>& mesh)
{
	m_mesh = mesh;
}

void MeshRendererComponent::setMaterial(const SharedPtr<Material>& material)
{
	m_material = material;
}