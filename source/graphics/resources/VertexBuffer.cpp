#include <graphics/resources/VertexBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_BUFFER_DESC createBufferDesc(const VertexBufferDesc& desc);
static D3D11_SUBRESOURCE_DATA createSubResourceData(const VertexBufferDesc& desc);

VertexBuffer::VertexBuffer(const VertexBufferDesc& vdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!vdesc.vertexCount) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex list size must be greater than zero.");
	}
	if (!vdesc.vertexSize) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex size must be be greater than zero.");
	}

	D3D11_BUFFER_DESC buffDesc = createBufferDesc(vdesc);
	D3D11_SUBRESOURCE_DATA initData = createSubResourceData(vdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc,
			(vdesc.vertexList) ? &initData : nullptr,
			&m_buffer
		),
		"CreateBuffer failed."
	);

	m_vertexCount = vdesc.vertexCount;
	m_vertexSize = vdesc.vertexSize;
}

VertexBuffer::~VertexBuffer() {}

uint32 VertexBuffer::getVertexCount() const noexcept
{
	return m_vertexCount;
}

/* STATIC FUNCTION DEFINITIONS */

static D3D11_BUFFER_DESC createBufferDesc(const VertexBufferDesc& desc)
{
	D3D11_BUFFER_DESC buffDesc{};

	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = desc.vertexCount * desc.vertexSize;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = 0;

	return buffDesc;
}

static D3D11_SUBRESOURCE_DATA createSubResourceData(const VertexBufferDesc& desc)
{
	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = desc.vertexList;
	return data;
}