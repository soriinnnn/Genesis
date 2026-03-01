#include <graphics/ConstantBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

D3D11_BUFFER_DESC createBufferDesc(const ConstantBufferDesc& desc);
D3D11_SUBRESOURCE_DATA createSubresourceData(const ConstantBufferDesc& desc);

ConstantBuffer::ConstantBuffer(const ConstantBufferDesc& cDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (!cDesc.bufferSize) {
		GENESIS_LOG_THROW_ERROR("Buffer size must be greater than zero.");
	}

	D3D11_BUFFER_DESC buffDesc = createBufferDesc(cDesc);
	D3D11_SUBRESOURCE_DATA initData = createSubresourceData(cDesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc, 
			(cDesc.buffer) ? &initData : nullptr,
			&m_buffer
		),
		"CreateBuffer failed."
	);

	m_size = cDesc.bufferSize;
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

D3D11_SUBRESOURCE_DATA createSubresourceData(const ConstantBufferDesc& desc)
{
	D3D11_SUBRESOURCE_DATA m_data{};
	m_data.pSysMem = desc.buffer;

	return m_data;
}