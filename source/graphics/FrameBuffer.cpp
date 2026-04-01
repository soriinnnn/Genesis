#include <graphics/FrameBuffer.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;

FrameBuffer::FrameBuffer(const FrameBufferDesc& desc): Base(desc.base), m_size{desc.size}
{
	if (m_size.width() == 0 || m_size.height() == 0) {
		GENESIS_LOG_THROW_ERROR("Invalid size: {}x{}.", m_size.width(), m_size.height());
	}
	m_graphicsDevice = &desc.graphicsDevice;
	updateTextures();
}

FrameBuffer::~FrameBuffer() {}

Rect FrameBuffer::getSize() const noexcept
{
	return m_size;
}

RenderTargetTexture& FrameBuffer::getRenderTarget() noexcept
{
	return *m_renderTarget;
}

DepthStencilTexture& FrameBuffer::getDepthStencil() noexcept
{
	return *m_depthStencil;
}

void FrameBuffer::setGraphicsDevice(GraphicsDevice& graphicsDevice)
{
	m_graphicsDevice = &graphicsDevice;
	updateTextures();
}

void FrameBuffer::resize(uint32 width, uint32 height)
{
	if (width == 0 || height == 0) {
		return;
	}
	m_size = Rect{static_cast<int32>(width), static_cast<int32>(height)};
	updateTextures();
}

void FrameBuffer::updateTextures()
{
	m_renderTarget = m_graphicsDevice->createRenderTargetTexture({m_size});
	m_depthStencil = m_graphicsDevice->createDepthStencilTexture({m_size});
}