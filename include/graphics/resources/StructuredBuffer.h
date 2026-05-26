#ifndef GENESIS_STRUCTURED_BUFFER_H
#define GENESIS_STRUCTURED_BUFFER_H
#include <graphics/resources/GraphicsResource.h>

GENESIS_NAMESPACE_START

struct StructuredBufferDesc
{
	const void* buffer{};
	uint32 structureByteStride{};
	uint32 structureCount{};
};

class StructuredBuffer final: public GraphicsResource
{
public:
	StructuredBuffer(const StructuredBufferDesc& bdesc, const GraphicsResourceDesc& gdesc);
	~StructuredBuffer() override;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_resourceView;
	uint32 m_size;

	friend class DeviceContext;
};

GENESIS_NAMESPACE_END

#endif