#ifndef GENESIS_CONSTANT_BUFFER_H
#define GENESIS_CONSTANT_BUFFER_H
#include <graphics/resources/GraphicsResource.h>

namespace genesis
{
	struct ConstantBufferDesc
	{
		const void* buffer{};
		uint32 bufferSize;
	};

	class ConstantBuffer final: public GraphicsResource
	{
	public:
		ConstantBuffer(const ConstantBufferDesc& cDesc, const GraphicsResourceDesc& grDesc);
		~ConstantBuffer() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		uint32 m_size;

		friend class DeviceContext;
	};
}


#endif