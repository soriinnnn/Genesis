#include <graphics/resources/StructuredBuffer.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_BUFFER_DESC createBufferDesc(const StructuredBufferDesc& desc);
static D3D11_SUBRESOURCE_DATA createSubResourceData(const StructuredBufferDesc& desc);
static D3D11_SHADER_RESOURCE_VIEW_DESC createShaderResourceViewDesc(const StructuredBufferDesc& desc);

StructuredBuffer::StructuredBuffer(const StructuredBufferDesc& bdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	D3D11_BUFFER_DESC buffDesc = createBufferDesc(bdesc);
	D3D11_SUBRESOURCE_DATA initData = createSubResourceData(bdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateBuffer(
			&buffDesc,
			(bdesc.buffer) ? &initData : nullptr,
			&m_buffer
		),
		"CreateBuffer failed."
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = createShaderResourceViewDesc(bdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateShaderResourceView(
			m_buffer.Get(), 
			&viewDesc, 
			&m_resourceView
		),
		"CreateShaderResourceView failed."
	);

	m_size = bdesc.structureCount * bdesc.structureByteStride;
}

StructuredBuffer::~StructuredBuffer() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_BUFFER_DESC createBufferDesc(const StructuredBufferDesc& desc)
{
	D3D11_BUFFER_DESC buffDesc{};

	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.ByteWidth = desc.structureCount * desc.structureByteStride;
	buffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buffDesc.StructureByteStride = desc.structureByteStride;

	return buffDesc;
}

D3D11_SUBRESOURCE_DATA createSubResourceData(const StructuredBufferDesc& desc)
{
	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = desc.buffer;
	return data;
}

D3D11_SHADER_RESOURCE_VIEW_DESC createShaderResourceViewDesc(const StructuredBufferDesc& desc)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{};

	viewDesc.Format = DXGI_FORMAT_UNKNOWN;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	viewDesc.Buffer.FirstElement = 0;
	viewDesc.Buffer.NumElements = desc.structureCount;

	return viewDesc;
}