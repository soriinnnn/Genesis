#include <graphics/resources/VertexBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

D3D11_BUFFER_DESC createBufferDesc(const VertexBufferDesc& desc);
D3D11_SUBRESOURCE_DATA createSubresourceData(const VertexBufferDesc& desc);

VertexBuffer::VertexBuffer(const VertexBufferDesc& vDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (!vDesc.vertexList) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex list is null.");
	}
	if (!vDesc.vertexCount) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex list size must be greater than zero.");
	}
	if (!vDesc.vertexSize) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex size must be be greater than zero.");
	}

	D3D11_BUFFER_DESC buffDesc = createBufferDesc(vDesc);
	D3D11_SUBRESOURCE_DATA initData = createSubresourceData(vDesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc,
			&initData,
			&m_buffer
		),
		"CreateBuffer failed."
	);

	m_vertexCount = vDesc.vertexCount;
	m_vertexSize = vDesc.vertexSize;
}

VertexBuffer::~VertexBuffer() {}

uint32 VertexBuffer::getVertexCount() const noexcept
{
	return m_vertexCount;
}

/* STATIC FUNCTION DEFINITIONS */

D3D11_BUFFER_DESC createBufferDesc(const VertexBufferDesc& desc) 
{
	D3D11_BUFFER_DESC buffDesc{};

	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = desc.vertexCount * desc.vertexSize;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	return buffDesc;
}

D3D11_SUBRESOURCE_DATA createSubresourceData(const VertexBufferDesc& desc) 
{
	D3D11_SUBRESOURCE_DATA m_data{};
	m_data.pSysMem = desc.vertexList;

	return m_data;
}