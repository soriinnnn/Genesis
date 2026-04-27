#include <entity/components/MeshRendererComponent.h>
#include <graphics/resources/GraphicsPipelineState.h>
#include <resources/Mesh.h>

using namespace genesis;

MeshRendererComponent::MeshRendererComponent(const ComponentDesc& desc): Component(desc) {}

MeshRendererComponent::~MeshRendererComponent() {}

Mesh* MeshRendererComponent::getMesh()
{
	return m_mesh.get();
}

Material* MeshRendererComponent::getMaterial()
{
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