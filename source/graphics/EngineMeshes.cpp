#include <graphics/EngineMeshes.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/utils/MeshPrimitives.h>

using namespace genesis;
using namespace meshPrimitives;

static EngineMeshes::EngineMesh createMesh(const MeshPrimitive& primitive, uint32 vertexSize, GraphicsDevice& graphicsDevice);

EngineMeshes::EngineMeshes(const EngineMeshesDesc& desc): Base(desc.base)
{
	m_skyboxSphere = createMesh(createSphere(), sizeof(Vertex), desc.graphicsDevice);
	m_skyboxCube = createMesh(createCube(), sizeof(Vertex), desc.graphicsDevice);
}

EngineMeshes::~EngineMeshes() {}

const EngineMeshes::EngineMesh& EngineMeshes::skyboxSphere() const
{
	return m_skyboxSphere;
}

const EngineMeshes::EngineMesh& EngineMeshes::skyboxCube() const
{
	return m_skyboxCube;
}

/* STATIC FUNCTION DEFINITIONS */

EngineMeshes::EngineMesh createMesh(const MeshPrimitive& primitive, uint32 vertexSize, GraphicsDevice& graphicsDevice)
{
	EngineMeshes::EngineMesh mesh;

	mesh.vertices = graphicsDevice.createVertexBuffer({primitive.vertices.data(), static_cast<uint32>(primitive.vertices.size()), vertexSize});
	mesh.indices = graphicsDevice.createIndexBuffer({primitive.indices.data(), static_cast<uint32>(primitive.indices.size())});

	return mesh;
}