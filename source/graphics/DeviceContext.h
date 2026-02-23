#ifndef GENESIS_DEVICE_CONTEXT_H
#define GENESIS_DEVICE_CONTEXT_H
#include <graphics/GraphicsResource.h>
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
		void setVertexBuffer(const VertexBuffer& buffer);
		void setViewport(const Rect& size);
		void draw(uint32 vertexCount, uint32 startVertexLocation);

	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;

		friend class GraphicsDevice;
	};
}

#endif