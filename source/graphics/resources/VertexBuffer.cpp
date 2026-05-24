#include <graphics/resources/VertexBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <graphics/utils/GraphicsUtils.h>

using namespace genesis;

static D3D11_BUFFER_DESC getD3D11BufferDesc(uint32 vertexCount, uint32 vertexSize, ResourceUsage usage);
static D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer);

VertexBuffer::VertexBuffer(const VertexBufferDesc& vdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_vertexCount{vdesc.vertexCount}, m_vertexSize{vdesc.vertexSize}, m_usage{vdesc.usage}
{
	if (m_vertexCount == 0) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex list size must be greater than zero.");
	}
	if (m_vertexSize == 0) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex size must be be greater than zero.");
	}
	if (vdesc.usage == ResourceUsage::Staging) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex buffer does not support Staging usage. Use Default, Immutable or Dynamic.");
	}

	D3D11_BUFFER_DESC buffDesc = getD3D11BufferDesc(m_vertexCount, m_vertexSize, m_usage);
	D3D11_SUBRESOURCE_DATA initData = getD3D11SubResourceData(vdesc.vertexList);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc,
			(vdesc.vertexList) ? &initData : nullptr,
			&m_buffer
		),
		"CreateBuffer failed."
	);
}

VertexBuffer::~VertexBuffer() {}

uint32 VertexBuffer::getVertexCount() const noexcept
{
	return m_vertexCount;
}

uint32 VertexBuffer::getVertexSize() const noexcept
{
	return m_vertexSize;
}

/* STATIC FUNCTION DEFINITIONS */

static D3D11_BUFFER_DESC getD3D11BufferDesc(uint32 vertexCount, uint32 vertexSize, ResourceUsage usage)
{
	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = static_cast<UINT>(vertexCount * vertexSize);
	desc.Usage = graphicsUtils::getD3D11Usage(usage);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = (usage == ResourceUsage::Dynamic) ? D3D11_CPU_ACCESS_WRITE : 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	return desc;
}

static D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer)
{
	D3D11_SUBRESOURCE_DATA data{};

	data.pSysMem = buffer;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	return data;
}