#include <graphics/resources/IndexBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static DXGI_FORMAT getD3D11IndexFormat(IndexFormat format);
static D3D11_BUFFER_DESC createBufferDesc(const IndexBufferDesc& desc);
static D3D11_SUBRESOURCE_DATA createSubResourceData(const IndexBufferDesc& desc);

IndexBuffer::IndexBuffer(const IndexBufferDesc& idesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!idesc.indexList) {
		GENESIS_LOG_THROW_INVALID_ARG("Index list is null.");
	}
	if (!idesc.indexCount) {
		GENESIS_LOG_THROW_INVALID_ARG("Index list size must be greater than zero.");
	}

	m_indexFormat = getD3D11IndexFormat(idesc.indexFormat);
	if (m_indexFormat == DXGI_FORMAT_UNKNOWN) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid index format.");
	}

	D3D11_BUFFER_DESC buffDesc = createBufferDesc(idesc);
	D3D11_SUBRESOURCE_DATA initData = createSubResourceData(idesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc,
			&initData,
			&m_buffer
		),
		"CreateBuffer failed."
	);

	m_indexCount = idesc.indexCount;
}

IndexBuffer::~IndexBuffer() {}

uint32 IndexBuffer::getIndexCount() const noexcept
{
	return m_indexCount;
}

/* STATIC FUNCTION DEFINITIONS */

static DXGI_FORMAT getD3D11IndexFormat(IndexFormat format)
{
	switch (format) {
	case IndexFormat::UnsignedInt16:
		return DXGI_FORMAT_R16_UINT;
	case IndexFormat::UnsignedInt32:
		return DXGI_FORMAT_R32_UINT;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

static D3D11_BUFFER_DESC createBufferDesc(const IndexBufferDesc& desc)
{
	D3D11_BUFFER_DESC buffDesc{};
	uint32 size = (desc.indexFormat == IndexFormat::UnsignedInt16) ? sizeof(uint16) : sizeof(uint32);
	
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = desc.indexCount * size;
	buffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;

	return buffDesc;
}

static D3D11_SUBRESOURCE_DATA createSubResourceData(const IndexBufferDesc& desc)
{
	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = desc.indexList;

	return data;
}