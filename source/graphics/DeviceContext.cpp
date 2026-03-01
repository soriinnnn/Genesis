#include <graphics/DeviceContext.h>
#include <graphics/SwapChain.h>
#include <graphics/GraphicsPipelineState.h>
#include <graphics/VertexBuffer.h>
#include <graphics/IndexBuffer.h>
#include <graphics/ConstantBuffer.h>
#include <graphics/GraphicsLogUtils.h>

using namespace genesis;
using namespace std;

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
	ID3D11RenderTargetView* renderTarget = swapChain.m_renderTarget.Get();

	m_context->ClearRenderTargetView(renderTarget, color.toArray());
	m_context->OMSetRenderTargets(1, &renderTarget, nullptr);
}

void DeviceContext::setGraphicsPipelineState(const GraphicsPipelineState& graphicsPipeline)
{
	m_context->IASetInputLayout(graphicsPipeline.m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(graphicsPipeline.m_primitive);
	m_context->VSSetShader(graphicsPipeline.m_vertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(graphicsPipeline.m_pixelShader.Get(), nullptr, 0);
}

void DeviceContext::setViewport(const Rect& size)
{
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<float>(size.width());
	viewport.Height = static_cast<float>(size.height());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_context->RSSetViewports(1, &viewport);
}

void DeviceContext::setVertexBuffer(const VertexBuffer& buffer)
{
	ID3D11Buffer* buff = buffer.m_buffer.Get();
	uint32 stride = buffer.m_vertexSize;
	uint32 offset = 0;

	m_context->IASetVertexBuffers(0, 1, &buff, &stride, &offset);
}

void DeviceContext::setIndexBuffer(const IndexBuffer& buffer)
{
	ID3D11Buffer* buff = buffer.m_buffer.Get();

	m_context->IASetIndexBuffer(buff, buffer.m_indexFormat, 0);
}

void DeviceContext::setConstantBuffer(const ConstantBuffer& buffer)
{
	ID3D11Buffer* buff = buffer.m_buffer.Get();

	m_context->VSSetConstantBuffers(0, 1, &buff);
	m_context->PSSetConstantBuffers(0, 1, &buff);
}
	
void DeviceContext::updateConstantBuffer(const ConstantBuffer& buffer, const void* m_data)
{
	if (!m_data) {
		GENESIS_LOG_THROW_ERROR("Null data pointer passed to updateConstantBuffer.");
	}

	ID3D11Buffer* buff = buffer.m_buffer.Get();
	D3D11_MAPPED_SUBRESOURCE mapped{};

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_context->Map(
			buff, 
			0,
			D3D11_MAP_WRITE_DISCARD, 
			0, 
			&mapped
		),
		"ID3D11DeviceContext::Map failed."
	);
	memcpy(mapped.pData, m_data, buffer.m_size);
	m_context->Unmap(buff, 0);
}

void DeviceContext::draw(uint32 vertexCount, uint32 startVertexLocation)
{
	m_context->Draw(vertexCount, startVertexLocation);
}

void DeviceContext::drawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseVertexLocation)
{
	m_context->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
