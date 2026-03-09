#ifndef GENESIS_VERTEX_BUFFER_H
#define GENESIS_VERTEX_BUFFER_H
#include <graphics/resources/GraphicsResource.h>

namespace genesis
{
	struct VertexBufferDesc
	{
		const void* vertexList{};
		uint32 vertexCount{};
		uint32 vertexSize{};
	};

	class VertexBuffer final: public GraphicsResource
	{
	public:
		VertexBuffer(const VertexBufferDesc& vDesc, const GraphicsResourceDesc& grDesc);
		~VertexBuffer() override;

		uint32 getVertexCount() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		uint32 m_vertexCount;
		uint32 m_vertexSize;

		friend class DeviceContext;
	};
}

#endif