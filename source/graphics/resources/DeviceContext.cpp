#include <graphics/resources/DeviceContext.h>
#include <graphics/resources/SwapChain.h>
#include <graphics/resources/RasterizerState.h>
#include <graphics/resources/DepthStencilState.h>
#include <graphics/resources/GraphicsPipelineState.h>
#include <graphics/resources/VertexBuffer.h>
#include <graphics/resources/IndexBuffer.h>
#include <graphics/resources/ConstantBuffer.h>
#include <graphics/resources/StructuredBuffer.h>
#include <graphics/resources/SamplerState.h>
#include <graphics/resources/ImageTexture.h>
#include <graphics/resources/DepthStencilTexture.h>
#include <graphics/resources/RenderTargetTexture.h>
#include <graphics/utils/GraphicsLogUtils.h>

#define DEFAULT_CONTEXT_FLAGS 0
#define DEFAULT_CLEAR_DEPTH 1.0f
#define DEFAULT_CLEAR_STENCIL 0
#define DEFAULT_MAP_FLAGS 0
#define DEFAULT_MIN_DEPTH 0.0f
#define DEFAULT_MAX_DEPTH 1.0f

using namespace genesis;
using namespace std;

DeviceContext::DeviceContext(const GraphicsResourceDesc& desc): GraphicsResource(desc)
{
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateDeferredContext(DEFAULT_CONTEXT_FLAGS, &m_context),
		"CreateDeferredContext failed."
	);
}

DeviceContext::~DeviceContext() {}

void DeviceContext::clearAndSetBackBuffer(const SwapChain& swapChain, const Vec4& color)
{
	ID3D11RenderTargetView* renderView = swapChain.m_targetView.Get();
	ID3D11DepthStencilView* depthView = swapChain.m_depthStencil->m_depthStencilView.Get();
	m_context->ClearRenderTargetView(renderView, color.toArray());
	m_context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, DEFAULT_CLEAR_DEPTH, DEFAULT_CLEAR_STENCIL);
	m_context->OMSetRenderTargets(1, &renderView, depthView);
}

void DeviceContext::clearRenderTarget(const RenderTargetTexture& renderTarget, const Vec4& color)
{
	ID3D11RenderTargetView* renderView = renderTarget.m_targetView.Get();
	m_context->ClearRenderTargetView(renderView, color.toArray());
}

void DeviceContext::clearDepthStencil(const DepthStencilTexture& depthStencil)
{
	ID3D11DepthStencilView* depthView = depthStencil.m_depthStencilView.Get();
	m_context->ClearDepthStencilView(depthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, DEFAULT_CLEAR_DEPTH, DEFAULT_CLEAR_STENCIL);
}

void DeviceContext::updateVertexBuffer(const VertexBuffer& buffer, const void* data, uint32 size)
{
	if (!data) {
		GENESIS_LOG_THROW_INVALID_ARG("Null data pointer.");
	}
	if (buffer.m_usage != ResourceUsage::Dynamic) {
		GENESIS_LOG_THROW_ERROR("Cannot update vertex buffer. Only Dynamic buffers can be mapped.");
	}
	if (size > buffer.getVertexCount() * buffer.getVertexSize()) {
		GENESIS_LOG_THROW_ERROR("Data size {} exceeds vertex buffer capacity ({} bytes).", size, buffer.getVertexCount() * buffer.getVertexSize());
	}

	ID3D11Buffer* buff = buffer.m_buffer.Get();
	D3D11_MAPPED_SUBRESOURCE mapped{};
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_context->Map(
			buff,
			0,
			D3D11_MAP_WRITE_DISCARD,
			DEFAULT_MAP_FLAGS,
			&mapped
		),
		"ID3D11DeviceContext::Map failed."
	);

	memcpy(mapped.pData, data, size);
	m_context->Unmap(buff, 0);
}

void DeviceContext::updateConstantBuffer(const ConstantBuffer& buffer, const void* data, uint32 size)
{
	if (!data) {
		GENESIS_LOG_THROW_INVALID_ARG("Null data pointer.");
	}
	if (size != buffer.m_size) {
		GENESIS_LOG_THROW_INVALID_ARG("Data size {} does not match constant buffer size ({} bytes).", size, buffer.m_size);
	}

	ID3D11Buffer* buff = buffer.m_buffer.Get();
	D3D11_MAPPED_SUBRESOURCE mapped{};
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_context->Map(
			buff,
			0,
			D3D11_MAP_WRITE_DISCARD,
			DEFAULT_MAP_FLAGS,
			&mapped
		),
		"ID3D11DeviceContext::Map failed."
	);

	memcpy(mapped.pData, data, size);
	m_context->Unmap(buff, 0);
}

void DeviceContext::updateStructuredBuffer(const StructuredBuffer& buffer, const void* data, uint32 size)
{
	if (!data) {
		GENESIS_LOG_THROW_INVALID_ARG("Null data pointer.");
	}
	if (size > buffer.m_size) {
		GENESIS_LOG_THROW_ERROR("Data size {} exceeds structured buffer capacity ({} bytes).", size, buffer.m_size);
	}

	ID3D11Buffer* buff = buffer.m_buffer.Get();
	D3D11_MAPPED_SUBRESOURCE mapped{};
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_context->Map(
			buff,
			0,
			D3D11_MAP_WRITE_DISCARD,
			DEFAULT_MAP_FLAGS,
			&mapped
		),
		"ID3D11DeviceContext::Map failed."
	);

	memcpy(mapped.pData, data, size);
	m_context->Unmap(buff, 0);
}

void DeviceContext::setRenderTarget(const RenderTargetTexture& renderTarget, const DepthStencilTexture& depthStencil)
{
	ID3D11RenderTargetView* renderView = renderTarget.m_targetView.Get();
	ID3D11DepthStencilView* depthView = depthStencil.m_depthStencilView.Get();
	m_context->OMSetRenderTargets(1, &renderView, depthView);
}

void DeviceContext::setRasterizerState(const RasterizerState& rasterizer)
{
	m_context->RSSetState(rasterizer.m_rasterizerState.Get());
}

void DeviceContext::setDepthStencilState(const DepthStencilState& depthStencil)
{
	m_context->OMSetDepthStencilState(depthStencil.m_depthStencilState.Get(), 0);
}

void DeviceContext::setGraphicsPipelineState(const GraphicsPipelineState& pipeline)
{
	m_context->IASetInputLayout(pipeline.m_inputLayout.Get());
	m_context->IASetPrimitiveTopology(pipeline.m_primitive);
	m_context->VSSetShader(pipeline.m_vertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(pipeline.m_pixelShader.Get(), nullptr, 0);
}

void DeviceContext::setViewport(const Rect& size)
{
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<float>(size.width());
	viewport.Height = static_cast<float>(size.height());
	viewport.MinDepth = DEFAULT_MIN_DEPTH;
	viewport.MaxDepth = DEFAULT_MAX_DEPTH;
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

void DeviceContext::setStructuredBuffer(const StructuredBuffer& buffer, uint32 slot)
{
	ID3D11ShaderResourceView* resourceView = buffer.m_resourceView.Get();
	m_context->VSSetShaderResources(slot, 1, &resourceView);
	m_context->PSSetShaderResources(slot, 1, &resourceView);
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

void DeviceContext::resolveTexture(const RenderTargetTexture& src, const RenderTargetTexture& dst)
{
	if (dst.m_sampleCount > 1) {
		GENESIS_LOG_THROW_ERROR("Cannot resolve texture: destination is multisampled.");
	}
	if (src.m_format != dst.m_format) {
		GENESIS_LOG_THROW_ERROR("Cannot resolve texture: source and destination formats do not match.");
	}
	ID3D11Resource* pSrc = src.m_texture.Get();
	ID3D11Resource* pDst = dst.m_texture.Get();
	m_context->ResolveSubresource(pDst, 0, pSrc, 0, src.m_format);
}