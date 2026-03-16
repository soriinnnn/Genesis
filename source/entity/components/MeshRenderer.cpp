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

Texture* MeshRenderer::getTexture()
{
	return m_texture.get();
}

GraphicsPipelineState* MeshRenderer::getGraphicsPipelineState()
{
	return m_pipeline.get();
}

void MeshRenderer::setMesh(const SharedPtr<Mesh>& mesh)
{
	m_mesh = mesh;
}

void MeshRenderer::setTexture(const SharedPtr<Texture>& texture)
{
	m_texture = texture;
}

void MeshRenderer::setGraphicsPipelineState(const SharedPtr<GraphicsPipelineState>& pipeline)
{
	m_pipeline = pipeline;
}