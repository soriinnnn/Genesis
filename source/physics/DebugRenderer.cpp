#include <physics/DebugRenderer.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/FrameBuffer.h>
#include <graphics/EngineShaders.h>

#define INITIAL_VERTEX_BUFFER_SIZE 100

using namespace genesis;

DebugRenderer::DebugRenderer(const DebugRendererDesc& desc): m_graphicsDevice{desc.graphicsContext.graphicsDevice}
{
	auto& debugShader = desc.graphicsContext.engineShaders.getDebugLine();
	m_pipeline = m_graphicsDevice.createGraphicsPipelineState({
		*debugShader.vsBinary,
		*debugShader.psBinary,
		*debugShader.vsSignature,
		*debugShader.psSignature,
		PrimitiveTopology::Lines,
		false
	});
	m_buffer = m_graphicsDevice.createVertexBuffer({nullptr, INITIAL_VERTEX_BUFFER_SIZE, sizeof(Vertex), ResourceUsage::Dynamic});
	m_vertices.reserve(INITIAL_VERTEX_BUFFER_SIZE);
	Initialize();
}

void DebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
{
	Vec4 color = {
		inColor.r / 255.0f, 
		inColor.g / 255.0f, 
		inColor.b / 255.0f, 
		inColor.a / 255.0f
	};
	m_vertices.push_back({{inFrom.GetX(), inFrom.GetY(), inFrom.GetZ()}, color});
	m_vertices.push_back({{inTo.GetX(), inTo.GetY(), inTo.GetZ()}, color});
}

void DebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
{
	DrawLine(inV1, inV2, inColor);
	DrawLine(inV2, inV3, inColor);
	DrawLine(inV3, inV1, inColor);
}

void DebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view& inString, JPH::ColorArg inColor, float inHeight)
{
	//printf("HOLA3");
}

/*
void DebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode) 
{
}

JPH::DebugRenderer::Batch DebugRenderer::CreateTriangleBatch(const JPH::DebugRenderer::Triangle* inTriangles, int inTriangleCount)
{
	return Batch();
}

JPH::DebugRenderer::Batch DebugRenderer::CreateTriangleBatch(const JPH::DebugRenderer::Vertex* inVertices, int inVertexCount, const uint32* inIndices, int inIndexCount)
{
	return Batch();
}
*/

void DebugRenderer::render(DeviceContext& context)
{
	context.setGraphicsPipelineState(*m_pipeline);

	uint32 vertexCount = static_cast<uint32>(m_vertices.size());
	if (vertexCount > m_buffer->getVertexCount()) {
		m_buffer = m_graphicsDevice.createVertexBuffer({nullptr, vertexCount * 2, sizeof(Vertex), ResourceUsage::Dynamic});
	}
	context.updateVertexBuffer(*m_buffer, m_vertices.data(), vertexCount * sizeof(Vertex));
	context.setVertexBuffer(*m_buffer);

	context.draw(vertexCount);
	m_vertices.clear();
}