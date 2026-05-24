#include <graphics/resources/ConstantBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static constexpr uint32 ELEMENT_SIZE = 16;
static constexpr uint32 CONSTANT_BUFFER_MAX_SIZE = ELEMENT_SIZE * D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT;

static D3D11_BUFFER_DESC getD3D11BufferDesc(uint32 size);
static D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer);

ConstantBuffer::ConstantBuffer(const ConstantBufferDesc& cdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_size{cdesc.size}
{
	if (m_size % ELEMENT_SIZE != 0) {
		GENESIS_LOG_THROW_INVALID_ARG("Buffer size must be a multiple of {} bytes. Provided size: {} bytes.", ELEMENT_SIZE, m_size);
	}
	if (m_size > CONSTANT_BUFFER_MAX_SIZE) {
		GENESIS_LOG_THROW_INVALID_ARG("Buffer size exceeds maximum allowed ({} bytes). Provided size: {} bytes.", CONSTANT_BUFFER_MAX_SIZE, m_size);
	}

	D3D11_BUFFER_DESC buffDesc = getD3D11BufferDesc(m_size);
	D3D11_SUBRESOURCE_DATA initData = getD3D11SubResourceData(cdesc.buffer);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc, 
			(cdesc.buffer) ? &initData : nullptr,
			&m_buffer
		),
		"CreateBuffer failed."
	);
}

ConstantBuffer::~ConstantBuffer() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_BUFFER_DESC getD3D11BufferDesc(uint32 size) {
	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = static_cast<UINT>(size);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	return desc;
}

D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer)
{
	D3D11_SUBRESOURCE_DATA data{};

	data.pSysMem = buffer;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	return data;
}