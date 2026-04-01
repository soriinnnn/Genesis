#include <graphics/resources/ConstantBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_BUFFER_DESC createBufferDesc(const ConstantBufferDesc& desc);
static D3D11_SUBRESOURCE_DATA createSubResourceData(const ConstantBufferDesc& desc);

ConstantBuffer::ConstantBuffer(const ConstantBufferDesc& cdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!cdesc.bufferSize) {
		GENESIS_LOG_THROW_ERROR("Buffer size must be greater than zero.");
	}

	D3D11_BUFFER_DESC buffDesc = createBufferDesc(cdesc);
	D3D11_SUBRESOURCE_DATA initData = createSubResourceData(cdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc, 
			(cdesc.buffer) ? &initData : nullptr,
			&m_buffer
		),
		"CreateBuffer failed."
	);

	m_size = cdesc.bufferSize;
}

ConstantBuffer::~ConstantBuffer() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_BUFFER_DESC createBufferDesc(const ConstantBufferDesc& desc) {
	D3D11_BUFFER_DESC buffDesc{};

	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = desc.bufferSize;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = 0;

	return buffDesc;
}

D3D11_SUBRESOURCE_DATA createSubResourceData(const ConstantBufferDesc& desc)
{
	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = desc.buffer;

	return data;
}