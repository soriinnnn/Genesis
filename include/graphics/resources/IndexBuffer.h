#ifndef GENESIS_INDEX_BUFFER_H
#define GENESIS_INDEX_BUFFER_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>

GENESIS_NAMESPACE_BEGIN

struct IndexBufferDesc
{
	const void* indexList{};
	uint32 indexCount{};
	IndexFormat indexFormat = IndexFormat::UnsignedInt32;
};

class IndexBuffer final: public GraphicsResource
{
public:
	IndexBuffer(const IndexBufferDesc& idesc, const GraphicsResourceDesc& gdesc);
	~IndexBuffer() override;

	uint32 getIndexCount() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	uint32 m_indexCount;
	DXGI_FORMAT m_indexFormat;

	friend class DeviceContext;
};

GENESIS_NAMESPACE_END

#endif