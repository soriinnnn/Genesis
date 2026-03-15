#ifndef GENESIS_MESH_H
#define GENESIS_MESH_H
#include <resources/Resource.h>

namespace genesis
{
	class Mesh final: public Resource
	{
	public:
		Mesh(const ResourceDesc& desc);
		~Mesh() override;

		VertexBuffer& getVertexBuffer();
		IndexBuffer& getIndexBuffer();

	private:
		void onLoad() override;
		void onUnload() override;

	private:
		SharedPtr<VertexBuffer> m_vertexBuffer;
		SharedPtr<IndexBuffer> m_indexBuffer;
	};
}

#endif