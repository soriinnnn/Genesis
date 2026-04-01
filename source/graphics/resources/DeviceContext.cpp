#include <graphics/resources/DeviceContext.h>
#include <graphics/resources/SwapChain.h>
#include <graphics/resources/GraphicsPipelineState.h>
#include <graphics/resources/VertexBuffer.h>
#include <graphics/resources/IndexBuffer.h>
#include <graphics/resources/ConstantBuffer.h>
#include <graphics/resources/SamplerState.h>
#include <graphics/resources/ImageTexture.h>
#include <graphics/resources/DepthStencilTexture.h>
#include <graphics/resources/RenderTargetTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>

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
	ID3D11RenderTargetView* renderView = swapChain.m_renderTarget.Get();
	ID3D11DepthStencilView* depthView = swapChain.m_depthStencil->m_depthStencilView.Get();

	m_context->ClearRenderTargetView(renderView, color.toArray());
	m_context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_context->OMSetRenderTargets(1, &renderView, depthView);
}

void DeviceContext::clearRenderTarget(const RenderTargetTexture& renderTarget, const Vec4& color)
{
	ID3D11RenderTargetView* renderView = renderTarget.m_renderTargetView.Get();
	m_context->ClearRenderTargetView(renderView, color.toArray());
}

void DeviceContext::clearDepthStencil(const DepthStencilTexture& depthStencil)
{
	ID3D11DepthStencilView* depthView = depthStencil.m_depthStencilView.Get();
	m_context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DeviceContext::setRenderTarget(const RenderTargetTexture& renderTarget, const DepthStencilTexture& depthStencil)
{
	ID3D11RenderTargetView* renderView = renderTarget.m_renderTargetView.Get();
	ID3D11DepthStencilView* depthView = depthStencil.m_depthStencilView.Get();
	m_context->OMSetRenderTargets(1, &renderView, depthView);
}

void DeviceContext::updateConstantBuffer(const ConstantBuffer& buffer, const void* data)
{
	if (!data) {
		GENESIS_LOG_THROW_ERROR("Null data pointer.");
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
	memcpy(mapped.pData, data, buffer.m_size);
	m_context->Unmap(buff, 0);
}

void DeviceContext::setGraphicsPipelineState(const GraphicsPipelineState& graphicsPipeline)
{
	m_context->IASetInputLayout(graphicsPipeline.m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(graphicsPipeline.m_primitive);
	m_context->VSSetShader(graphicsPipeline.m_vertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(graphicsPipeline.m_pixelShader.Get(), nullptr, 0);
	m_context->OMSetDepthStencilState(graphicsPipeline.m_depthStencilState.Get(), 0);
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

void DeviceContext::setConstantBuffer(const ConstantBuffer& buffer, uint32 slot)
{
	ID3D11Buffer* buff = buffer.m_buffer.Get();
	m_context->VSSetConstantBuffers(slot, 1, &buff);
	m_context->PSSetConstantBuffers(slot, 1, &buff);
}

void DeviceContext::setTexture(const ImageTexture& texture, uint32 slot)
{
	ID3D11ShaderResourceView* resourceView = texture.m_resourceView.Get();
	m_context->VSSetShaderResources(slot, 1, &resourceView);
	m_context->PSSetShaderResources(slot, 1, &resourceView);
}

void genesis::DeviceContext::setTexture(const RenderTargetTexture& texture, uint32 slot)
{
	ID3D11ShaderResourceView* resourceView = texture.m_resourceView.Get();
	m_context->VSSetShaderResources(slot, 1, &resourceView);
	m_context->PSSetShaderResources(slot, 1, &resourceView);
}

void DeviceContext::setSamplerState(const SamplerState& sampler, uint32 slot)
{
	ID3D11SamplerState* samplerState = sampler.m_samplerState.Get();
	m_context->VSSetSamplers(slot, 1, &samplerState);
	m_context->PSSetSamplers(slot, 1, &samplerState);
}

void DeviceContext::draw(uint32 vertexCount, uint32 startVertexLocation)
{
	m_context->Draw(vertexCount, startVertexLocation);
}

void DeviceContext::drawIndexed(uint32 indexCount, uint32 startIndexLocation, int32 baseVertexLocation)
{
	m_context->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
