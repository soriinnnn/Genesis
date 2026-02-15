#include <graphics/VertexBuffer.h>
#include <graphics/GraphicsLogUtils.h>

using namespace genesis;

D3D11_BUFFER_DESC createBufferDesc(const VertexBufferDesc& desc);
D3D11_SUBRESOURCE_DATA createSubresourceData(const VertexBufferDesc& desc);

// --------------------------------------------------------------------------------

VertexBuffer::VertexBuffer(const VertexBufferDesc& vDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (!vDesc.vertexList) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex list is null.");
	}
	if (!vDesc.vertexListSize) {
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

	m_vertexSize = vDesc.vertexSize;
}

VertexBuffer::~VertexBuffer() {}

// --------------------------------------------------------------------------------

D3D11_BUFFER_DESC createBufferDesc(const VertexBufferDesc& desc) 
{
	D3D11_BUFFER_DESC buffDesc{};

	buffDesc.ByteWidth = desc.vertexListSize * desc.vertexSize;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	return buffDesc;
}

D3D11_SUBRESOURCE_DATA createSubresourceData(const VertexBufferDesc& desc) 
{
	D3D11_SUBRESOURCE_DATA data{};

	data.pSysMem = desc.vertexList;

	return data;
}