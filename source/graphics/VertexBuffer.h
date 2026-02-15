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

		unsigned int getVertexListSize() const noexcept;


	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		unsigned int m_vertexListSize;
		unsigned int m_vertexSize;

		friend class DeviceContext;
	};
}

#endif