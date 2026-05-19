#ifndef GENESIS_FRAME_BUFFER_H
#define GENESIS_FRAME_BUFFER_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Rect.h>

namespace genesis
{
	struct FrameBufferDesc
	{
		BaseDesc base;
		Rect size;
		GraphicsDevice& graphicsDevice;
	};

	class FrameBuffer final: public Base
	{
	public:
		explicit FrameBuffer(const FrameBufferDesc& desc);
		~FrameBuffer() override;

		Rect getSize() const noexcept;
		const RenderTargetTexture& getRenderTarget() const noexcept;
		const DepthStencilTexture& getDepthStencil() const noexcept;

		void setGraphicsDevice(GraphicsDevice& graphicsDevice);
		void resize(uint32 width, uint32 height);

	private:
		void updateTextures();

	private:
		SharedPtr<RenderTargetTexture> m_renderTarget;
		SharedPtr<DepthStencilTexture> m_depthStencil;
		Rect m_size;
		GraphicsDevice* m_graphicsDevice;
	};
}

#endif