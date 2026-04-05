#ifndef GENESIS_DEVICE_CONTEXT_H
#define GENESIS_DEVICE_CONTEXT_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>
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
		void clearRenderTarget(const RenderTargetTexture& renderTarget, const Vec4& color);
		void clearDepthStencil(const DepthStencilTexture& depthStencil);
		void setRenderTarget(const RenderTargetTexture& renderTarget, const DepthStencilTexture& depthStencil);
		void updateConstantBuffer(const ConstantBuffer& buffer, const void* data);
		void updateStructuredBuffer(const StructuredBuffer& buffer, const void* data, uint32 dataSize);
		void setGraphicsPipelineState(const GraphicsPipelineState& graphicsPipeline);
		void setViewport(const Rect& size);
		void setVertexBuffer(const VertexBuffer& buffer);
		void setIndexBuffer(const IndexBuffer& buffer);
		void setConstantBuffer(const ConstantBuffer& buffer, uint32 slot = 0);
		void setStructuredBuffer(const StructuredBuffer& buffer, uint32 slot = 0);
		void setTexture(const ImageTexture& texture, uint32 slot = 0);
		void setTexture(const RenderTargetTexture& texture, uint32 slot = 0);
		void setSamplerState(const SamplerState& sampler, uint32 slot = 0);
		void draw(uint32 vertexCount, uint32 startVertexLocation = 0);
		void drawIndexed(uint32 indexCount, uint32 startIndexLocation = 0, int32 baseVertexLocation = 0);

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;

		friend class GraphicsDevice;
		friend class SpriteBatch;
	};
}

#endif