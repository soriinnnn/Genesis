#ifndef GENESIS_MESH_H
#define GENESIS_MESH_H
#include <resources/Resource.h>

namespace genesis
{
	struct MeshDesc 
	{
		GraphicsDevice& graphicsDevice;
	};

	class Mesh final: public Resource
	{
	public:
		Mesh(const MeshDesc& mdesc, const ResourceDesc& rdesc);
		~Mesh() override;

		VertexBuffer& getVertexBuffer();
		IndexBuffer& getIndexBuffer();

	private:
		void onLoad() override;
		void onUnload() override;

	private:
		GraphicsDevice& m_graphicsDevice;
		SharedPtr<VertexBuffer> m_vertexBuffer;
		SharedPtr<IndexBuffer> m_indexBuffer;
	};
}

#endif