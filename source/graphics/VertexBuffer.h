#ifndef GENESIS_VERTEX_BUFFER_H
#define GENESIS_VERTEX_BUFFER_H
#include <graphics/GraphicsResource.h>

namespace genesis
{
	class VertexBuffer final: public GraphicsResource
	{
	public:
		VertexBuffer(const VertexBufferDesc& vDesc, const GraphicsResourceDesc& grDesc);
		~VertexBuffer() override;

		uint32 getVertexListSize() const noexcept;


	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		uint32 m_vertexListSize;
		uint32 m_vertexSize;

		friend class DeviceContext;
	};
}

#endif