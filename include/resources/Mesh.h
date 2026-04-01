#ifndef GENESIS_MESH_H
#define GENESIS_MESH_H
#include <resources/Resource.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
	struct MeshDesc
	{
		ResourceDesc resource;
		uint32 components = GENESIS_VERTEX_PRESET_DEFAULT;
	};

	class Mesh final: public Resource
	{
	public:
		explicit Mesh(const MeshDesc& desc);
		~Mesh() override;

		VertexBuffer& getVertexBuffer() noexcept;
		IndexBuffer& getIndexBuffer() noexcept;

	private:
		SharedPtr<VertexBuffer> m_vertexBuffer;
		SharedPtr<IndexBuffer> m_indexBuffer;
	};
}

#endif