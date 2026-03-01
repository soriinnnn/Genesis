#include <graphics/IndexBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

DXGI_FORMAT getD3D11IndexFormat(IndexFormat format);
D3D11_BUFFER_DESC createBufferDesc(const IndexBufferDesc& desc);
D3D11_SUBRESOURCE_DATA createSubresourceData(const IndexBufferDesc& desc);

IndexBuffer::IndexBuffer(const IndexBufferDesc& iDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (!iDesc.indexList) {
		GENESIS_LOG_THROW_INVALID_ARG("Index list is null.");
	}
	if (!iDesc.indexCount) {
		GENESIS_LOG_THROW_INVALID_ARG("Index list size must be greater than zero.");
	}

	m_indexFormat = getD3D11IndexFormat(iDesc.indexFormat);
	if (m_indexFormat == DXGI_FORMAT_UNKNOWN) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid index format.");
	}

	D3D11_BUFFER_DESC buffDesc = createBufferDesc(iDesc);
	D3D11_SUBRESOURCE_DATA initData = createSubresourceData(iDesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc,
			&initData,
			&m_buffer
		),
		"CreateBuffer failed."
	);

	m_indexCount = static_cast<uint32>(iDesc.indexCount);
}

IndexBuffer::~IndexBuffer() {}

uint32 IndexBuffer::getIndexCount() const noexcept
{
	return m_indexCount;
}

/* STATIC FUNCTION DEFINITIONS */

DXGI_FORMAT getD3D11IndexFormat(IndexFormat format)
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

D3D11_BUFFER_DESC createBufferDesc(const IndexBufferDesc& desc)
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

D3D11_SUBRESOURCE_DATA createSubresourceData(const IndexBufferDesc& desc)
{
	D3D11_SUBRESOURCE_DATA m_data{};
	m_data.pSysMem = desc.indexList;

	return m_data;
}