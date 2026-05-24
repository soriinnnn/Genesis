#include <graphics/resources/StructuredBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_BUFFER_DESC getD3D11BufferDesc(uint32 structureCount, uint32 structureByteStride);
static D3D11_SUBRESOURCE_DATA getD3D11SubResourceData(const void* buffer);
static D3D11_SHADER_RESOURCE_VIEW_DESC getD3D11ShaderResourceViewDesc(uint32 structureCount);

StructuredBuffer::StructuredBuffer(const StructuredBufferDesc& bdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc), m_size{bdesc.structureCount * bdesc.structureByteStride}
{
	if (bdesc.structureCount == 0) {
		GENESIS_LOG_THROW_INVALID_ARG("Structure count must be greater than zero.");
	}
	if (bdesc.structureByteStride > D3D11_REQ_MULTI_ELEMENT_STRUCTURE_SIZE_IN_BYTES) {
		GENESIS_LOG_THROW_INVALID_ARG("Structure byte stride {} exceeds maximum ({}).", bdesc.structureByteStride, D3D11_REQ_MULTI_ELEMENT_STRUCTURE_SIZE_IN_BYTES);
	}

	D3D11_BUFFER_DESC buffDesc = getD3D11BufferDesc(bdesc.structureCount, bdesc.structureByteStride);
	D3D11_SUBRESOURCE_DATA initData = getD3D11SubResourceData(bdesc.buffer);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc,
			(bdesc.buffer) ? &initData : nullptr,
			&m_buffer
		),
		"CreateBuffer failed."
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = getD3D11ShaderResourceViewDesc(bdesc.structureCount);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(
			m_buffer.Get(), 
			&resourceDesc, 
			&m_resourceView
		),
		"CreateShaderResourceView failed."
	);
}

StructuredBuffer::~StructuredBuffer() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_BUFFER_DESC getD3D11BufferDesc(uint32 structureCount, uint32 structureByteStride)
{
	D3D11_BUFFER_DESC desc{};

	desc.ByteWidth = static_cast<UINT>(structureCount * structureByteStride);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = static_cast<UINT>(structureByteStride);

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

D3D11_SHADER_RESOURCE_VIEW_DESC getD3D11ShaderResourceViewDesc(uint32 structureCount)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};

	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	desc.Buffer.NumElements = static_cast<UINT>(structureCount);

	return desc;
}