#ifndef GENESIS_ENGINE_MESHES_H
#define GENESIS_ENGINE_MESHES_H
#include <core/Core.h>
#include <core/Base.h>

GENESIS_NAMESPACE_BEGIN

struct EngineMeshesDesc
{
	BaseDesc base;
	GraphicsDevice& graphicsDevice;
};

class EngineMeshes final: public Base
{
public:
	struct EngineMesh
	{
		SharedPtr<VertexBuffer> vertices;
		SharedPtr<IndexBuffer> indices;
	};

public:
	explicit EngineMeshes(const EngineMeshesDesc& desc);
	~EngineMeshes() override;

	const EngineMesh& skyboxSphere() const;
	const EngineMesh& skyboxCube() const;

private:
	EngineMesh m_skyboxSphere;
	EngineMesh m_skyboxCube;
};

GENESIS_NAMESPACE_END

#endif