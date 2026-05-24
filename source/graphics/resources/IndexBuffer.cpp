#include <graphics/resources/IndexBuffer.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_BUFFER_DESC getD3D11BufferDesc(uint32 indexCount, IndexFormat indexFormat);
static D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer);

IndexBuffer::IndexBuffer(const IndexBufferDesc& idesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_indexCount{idesc.indexCount}
{
	if (!idesc.indexList) {
		GENESIS_LOG_THROW_INVALID_ARG("Index list is null.");
	}
	if (!idesc.indexCount) {
		GENESIS_LOG_THROW_INVALID_ARG("Index list size must be greater than zero.");
	}

	m_indexFormat = graphicsUtils::getDXGIIndexFormat(idesc.indexFormat);
	if (m_indexFormat == DXGI_FORMAT_UNKNOWN) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid index format.");
	}

	D3D11_BUFFER_DESC buffDesc = getD3D11BufferDesc(idesc.indexCount, idesc.indexFormat);
	D3D11_SUBRESOURCE_DATA initData = getD3D11SubResourceData(idesc.indexList);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(&buffDesc, &initData, &m_buffer),
		"CreateBuffer failed."
	);
}

IndexBuffer::~IndexBuffer() {}

uint32 IndexBuffer::getIndexCount() const noexcept
{
	return m_indexCount;
}

/* STATIC FUNCTION DEFINITIONS */

static D3D11_BUFFER_DESC getD3D11BufferDesc(uint32 indexCount, IndexFormat indexFormat)
{
	D3D11_BUFFER_DESC desc{};
	
	desc.ByteWidth = static_cast<UINT>(indexCount * graphicsUtils::getIndexSize(indexFormat));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
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