#ifndef GENESIS_GRAPHICS_DEVICE_H
#define GENESIS_GRAPHICS_DEVICE_H
#include <core/Base.h>
#include <core/Core.h>
#include <graphics/resources/GraphicsResource.h>
#include <graphics/resources/DeviceContext.h>
#include <graphics/resources/SwapChain.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/resources/ShaderSignature.h>
#include <graphics/resources/VertexBuffer.h>
#include <graphics/resources/IndexBuffer.h>
#include <graphics/resources/ConstantBuffer.h>
#include <graphics/resources/StructuredBuffer.h>
#include <graphics/resources/RasterizerState.h>
#include <graphics/resources/DepthStencilState.h>
#include <graphics/resources/GraphicsPipelineState.h>
#include <graphics/resources/SamplerState.h>
#include <graphics/resources/ImageTexture.h>
#include <graphics/resources/DepthStencilTexture.h>
#include <graphics/resources/RenderTargetTexture.h>
#include <graphics/resources/SpriteBatch.h>
#include <graphics/resources/FontAtlas.h>
#include <d3d11.h>
#include <wrl.h>

GENESIS_NAMESPACE_START

struct GraphicsDeviceDesc
{
    BaseDesc base;
};

class GraphicsDevice: public Base, public std::enable_shared_from_this<GraphicsDevice>
{
public:
    explicit GraphicsDevice(const GraphicsDeviceDesc& desc);
    virtual ~GraphicsDevice() override;

    SharedPtr<DeviceContext> createDeviceContext();
    SharedPtr<SwapChain> createSwapChain(const SwapChainDesc& desc);

    SharedPtr<ShaderBinary> createShaderBinary(const ShaderBinaryDesc& desc);
    SharedPtr<ShaderBinary> compileShader(const ShaderCompileDesc& desc);
    SharedPtr<ShaderSignature> reflectShader(const ShaderSignatureDesc& desc);

    SharedPtr<VertexBuffer> createVertexBuffer(const VertexBufferDesc& desc);
    SharedPtr<IndexBuffer> createIndexBuffer(const IndexBufferDesc& desc);
    SharedPtr<ConstantBuffer> createConstantBuffer(const ConstantBufferDesc& desc);
    SharedPtr<StructuredBuffer> createStructuredBuffer(const StructuredBufferDesc& desc);

    SharedPtr<RasterizerState> createRasterizerState(const RasterizerStateDesc& desc);
    SharedPtr<DepthStencilState> createDepthStencilState(const DepthStencilStateDesc& desc);
    SharedPtr<GraphicsPipelineState> createGraphicsPipelineState(const GraphicsPipelineStateDesc& desc);
    SharedPtr<SamplerState> createSamplerState(const SamplerStateDesc& desc);

    SharedPtr<ImageTexture> createImageTexture(const ImageTextureFileDesc& desc);
    SharedPtr<ImageTexture> createImageTexture(const ImageTextureSolidDesc& desc);
    SharedPtr<DepthStencilTexture> createDepthStencilTexture(const DepthStencilTextureDesc& desc);
    SharedPtr<RenderTargetTexture> createRenderTargetTexture(const RenderTargetTextureDesc& desc);

    SharedPtr<SpriteBatch> createSpriteBatch(const SpriteBatchDesc& desc);
    SharedPtr<FontAtlas> createFontAtlas(const FontAtlasDesc& desc);

    void clearCache();
    void clearState();
    void clearCommandList(DeviceContext& context);
    void executeCommandList(DeviceContext& context);

private:
    GraphicsResourceDesc getGraphicsResourceDesc() noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
    Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgiDevice;
    Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgiAdapter;
    Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgiFactory;
    UniquePtr<PipelineStateCache> m_pipelineCache;
    UniquePtr<SamplerStateCache> m_samplerCache;

    friend class GraphicsCache;
};

GENESIS_NAMESPACE_END

#endif