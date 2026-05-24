#ifndef GENESIS_VERTEX_BUFFER_H
#define GENESIS_VERTEX_BUFFER_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
	struct VertexBufferDesc
	{
		const void* vertexList{};
		uint32 vertexCount{};
		uint32 vertexSize{};
		ResourceUsage usage = ResourceUsage::Default;
	};

	class VertexBuffer final: public GraphicsResource
	{
	public:
		VertexBuffer(const VertexBufferDesc& vdesc, const GraphicsResourceDesc& gdesc);
		~VertexBuffer() override;

		uint32 getVertexCount() const noexcept;
		uint32 getVertexSize() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		ResourceUsage m_usage;
		uint32 m_vertexCount;
		uint32 m_vertexSize;

		friend class DeviceContext;
	};
}

#endif