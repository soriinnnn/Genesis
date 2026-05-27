#ifndef GENESIS_FRAME_BUFFER_H
#define GENESIS_FRAME_BUFFER_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Rect.h>

GENESIS_NAMESPACE_BEGIN

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
	uint32 getSampleCount() const noexcept;
	const RenderTargetTexture& getRenderTarget() const noexcept;
	const DepthStencilTexture& getDepthStencil() const noexcept;

	void setSize(const Rect& size);
	void setSampleCount(uint32 sampleCount);

private:
	void updateTextures();

private:
	GraphicsDevice& m_graphicsDevice;
	SharedPtr<RenderTargetTexture> m_renderTarget;
	SharedPtr<DepthStencilTexture> m_depthStencil;
	Rect m_size;
	uint32 m_sampleCount;
};

GENESIS_NAMESPACE_END

#endif