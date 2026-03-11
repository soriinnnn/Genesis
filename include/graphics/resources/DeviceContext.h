#ifndef GENESIS_DEVICE_CONTEXT_H
#define GENESIS_DEVICE_CONTEXT_H
#include <graphics/resources/GraphicsResource.h>
#include <math/Rect.h>
#include <math/Vec4.h>

namespace genesis 
{
	class DeviceContext final: public GraphicsResource
	{
	public:
		explicit DeviceContext(const GraphicsResourceDesc& desc);
		~DeviceContext() override;

		void clearAndSetBackBuffer(const SwapChain& swapChain, const Vec4& color);
		void setGraphicsPipelineState(const GraphicsPipelineState& graphicsPipeline);
		void setViewport(const Rect& size);
		void setVertexBuffer(const VertexBuffer& buffer);
		void setIndexBuffer(const IndexBuffer& buffer);
		void setConstantBuffer(const ConstantBuffer& buffer);
		void updateConstantBuffer(const ConstantBuffer& buffer, const void* m_data);
		void setTexture(const GraphicsTexture& texture);

		void draw(uint32 vertexCount, uint32 startVertexLocation = 0);
		void drawIndexed(uint32 indexCount, uint32 startIndexLocation = 0, int32 baseVertexLocation = 0);

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;

		friend class GraphicsDevice;
	};
}

#endif