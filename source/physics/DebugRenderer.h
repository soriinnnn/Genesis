#ifndef GENESIS_DEBUG_RENDERER_H
#define GENESIS_DEBUG_RENDERER_H
#include <core/Core.h>
#include <graphics/utils/GraphicsTypes.h>
#include <math/Vec3.h>
#include <math/Vec4.h>
#include <jolt/Jolt.h>
#include <jolt/Renderer/DebugRendererSimple.h>

GENESIS_NAMESPACE_START

struct DebugRendererDesc
{
	GraphicsContext graphicsContext;
};

class DebugRenderer final: public JPH::DebugRendererSimple
{
public:
	explicit DebugRenderer(const DebugRendererDesc& desc);

	void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;
	void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow = ECastShadow::Off) override;
	void DrawText3D(JPH::RVec3Arg inPosition, const JPH::string_view& inString, JPH::ColorArg inColor = JPH::Color::sWhite, float inHeight = 0.5f) override;

	//void DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode = ECullMode::CullBackFace, ECastShadow inCastShadow = ECastShadow::On, EDrawMode inDrawMode = EDrawMode::Solid) override;
	//Batch CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount) override;
	//Batch CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const uint32* inIndices, int inIndexCount) override;

	void render(DeviceContext& context);

private:
	struct Vertex
	{
		Vec3 position;
		Vec4 color;
	};

private:
	GraphicsDevice& m_graphicsDevice;
	SharedPtr<GraphicsPipelineState> m_pipeline;
	SharedPtr<VertexBuffer> m_buffer;
	Vector<Vertex> m_vertices;
};

GENESIS_NAMESPACE_END

#endif