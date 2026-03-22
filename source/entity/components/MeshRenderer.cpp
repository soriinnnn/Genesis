#include <entity/components/MeshRenderer.h>
#include <graphics/resources/GraphicsPipelineState.h>
#include <resources/Mesh.h>

using namespace genesis;

MeshRenderer::MeshRenderer(const ComponentDesc& desc): Component(desc) {}

MeshRenderer::~MeshRenderer() {}

Mesh* MeshRenderer::getMesh()
{
	return m_mesh.get();
}

Material* MeshRenderer::getMaterial()
{
	return m_material.get();
}

void MeshRenderer::setMesh(const SharedPtr<Mesh>& mesh)
{
	m_mesh = mesh;
}

void MeshRenderer::setMaterial(const SharedPtr<Material>& material)
{
	m_material = material;
}