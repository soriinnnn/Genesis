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

const RenderTargetTexture& FrameBuffer::getRenderTarget() const noexcept
{
	return *m_renderTarget;
}

const DepthStencilTexture& FrameBuffer::getDepthStencil() const noexcept
{
	return *m_depthStencil;
}

void FrameBuffer::setGraphicsDevice(GraphicsDevice& graphicsDevice)
{
	m_graphicsDevice = &graphicsDevice;
	updateTextures();
}

void FrameBuffer::setSize(const Rect& size)
{
	if (size.width() <= 0 || size.height() <= 0) {
		return;
	}
	if (m_size == size) {
		return;
	}
	m_size = size;
	updateTextures();
}

void FrameBuffer::updateTextures()
{
	m_renderTarget = m_graphicsDevice->createRenderTargetTexture({m_size});
	m_depthStencil = m_graphicsDevice->createDepthStencilTexture({m_size});
}