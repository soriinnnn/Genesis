#include <graphics/DeviceContext.h>
#include <graphics/SwapChain.h>
#include <graphics/GraphicsPipelineState.h>
#include <graphics/GraphicsLogUtils.h>
#include <graphics/VertexBuffer.h>

using namespace genesis;

// --------------------------------------------------------------------------------

DeviceContext::DeviceContext(const GraphicsResourceDesc& desc): GraphicsResource(desc)
{
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateDeferredContext(0, &m_context),
		"CreateDeferredContext failed."
	);
}

DeviceContext::~DeviceContext() {}

void DeviceContext::clearAndSetBackBuffer(const SwapChain& swapChain, const Vec4& color)
{
	auto renderTarget = swapChain.m_renderTarget.Get();

	m_context->ClearRenderTargetView(renderTarget, color.toArray());
	m_context->OMSetRenderTargets(1, &renderTarget, nullptr);
}

void DeviceContext::setGraphicsPipelineState(const GraphicsPipelineState& graphicsPipeline)
{
	m_context->IASetInputLayout(graphicsPipeline.m_inputLayout.Get());
	m_context->VSSetShader(graphicsPipeline.m_vertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(graphicsPipeline.m_pixelShader.Get(), nullptr, 0);
}

void DeviceContext::setVertexBuffer(const VertexBuffer& buffer)
{
	auto buff = buffer.m_buffer.Get();
	unsigned int stride = buffer.m_vertexSize;
	unsigned int offset = 0;

	m_context->IASetVertexBuffers(0, 1, &buff, &stride, &offset);
}

void DeviceContext::setViewportSize(const Rect& size)
{
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<float>(size.width());
	viewport.Height = static_cast<float>(size.height());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_context->RSSetViewports(1, &viewport);
}

void DeviceContext::drawTriangleList(unsigned int vertexCount, unsigned int startVertexLocation)
{
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->Draw(vertexCount, startVertexLocation);
}
