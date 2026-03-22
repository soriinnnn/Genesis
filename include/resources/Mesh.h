#ifndef GENESIS_MESH_H
#define GENESIS_MESH_H
#include <resources/Resource.h>

namespace genesis
{
	struct MeshDesc
	{
		ResourceDesc resource;
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