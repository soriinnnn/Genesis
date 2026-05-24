#include <graphics/GraphicsDevice.h>
#include <graphics/cache/PipelineStateCache.h>
#include <graphics/cache/SamplerStateCache.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;
using namespace std;

GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc): Base(desc.base)
{
    D3D_FEATURE_LEVEL featureLevel;
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
        D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &m_d3dDevice,
            &featureLevel,
            &m_d3dContext
        ),
        "Direct3D11 initialitzation failed."
    );

    GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
        m_d3dDevice->QueryInterface(IID_PPV_ARGS(&m_dxgiDevice)),
        "Failed to retrieve IDXGIDevice."
    );

    GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
        m_dxgiDevice->GetParent(IID_PPV_ARGS(&m_dxgiAdapter)),
        "Failed to retrieve IDXGIAdapter."
    );

    GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
        m_dxgiAdapter->GetParent(IID_PPV_ARGS(&m_dxgiFactory)),
        "Failed to retrieve IDXGIFactory."
    );

    m_pipelineCache = make_unique<PipelineStateCache>(GraphicsCacheDesc{m_logger, *this});
    m_samplerCache = make_unique<SamplerStateCache>(GraphicsCacheDesc{m_logger, *this});
}

GraphicsDevice::~GraphicsDevice() {}

SharedPtr<DeviceContext> GraphicsDevice::createDeviceContext()
{
    return make_shared<DeviceContext>(getGraphicsResourceDesc());
}

SharedPtr<SwapChain> GraphicsDevice::createSwapChain(const SwapChainDesc& desc)
{
    return make_shared<SwapChain>(desc, getGraphicsResourceDesc());
}

SharedPtr<ShaderBinary> GraphicsDevice::createShaderBinary(const ShaderBinaryDesc& desc)
{
    return make_shared<ShaderBinary>(desc, getGraphicsResourceDesc());
}

SharedPtr<ShaderBinary> GraphicsDevice::compileShader(const ShaderCompileDesc& desc)
{
    return make_shared<ShaderBinary>(desc, getGraphicsResourceDesc());
}

SharedPtr<ShaderSignature> GraphicsDevice::reflectShader(const ShaderSignatureDesc& desc)
{
    return make_shared<ShaderSignature>(desc, getGraphicsResourceDesc());
}

SharedPtr<VertexBuffer> GraphicsDevice::createVertexBuffer(const VertexBufferDesc& desc)
{
    return make_shared<VertexBuffer>(desc, getGraphicsResourceDesc());
}

SharedPtr<IndexBuffer> GraphicsDevice::createIndexBuffer(const IndexBufferDesc& desc)
{
    return make_shared<IndexBuffer>(desc, getGraphicsResourceDesc());
}

SharedPtr<ConstantBuffer> GraphicsDevice::createConstantBuffer(const ConstantBufferDesc& desc)
{
    return make_shared<ConstantBuffer>(desc, getGraphicsResourceDesc());
}

SharedPtr<StructuredBuffer> GraphicsDevice::createStructuredBuffer(const StructuredBufferDesc& desc)
{
    return make_shared<StructuredBuffer>(desc, getGraphicsResourceDesc());
}

SharedPtr<GraphicsPipelineState> GraphicsDevice::createGraphicsPipelineState(const GraphicsPipelineStateDesc& desc)
{
    return m_pipelineCache->get(desc);
}

SharedPtr<SamplerState> GraphicsDevice::createSamplerState(const SamplerStateDesc& desc)
{
    return m_samplerCache->get(desc);
}

SharedPtr<ImageTexture> GraphicsDevice::createImageTexture(const ImageTextureFileDesc& desc)
{
    return make_shared<ImageTexture>(desc, getGraphicsResourceDesc());
}

SharedPtr<ImageTexture> GraphicsDevice::createImageTexture(const ImageTextureSolidDesc& desc)
{
    return make_shared<ImageTexture>(desc, getGraphicsResourceDesc());
}

SharedPtr<DepthStencilTexture> GraphicsDevice::createDepthStencilTexture(const DepthStencilTextureDesc& desc)
{
    return make_shared<DepthStencilTexture>(desc, getGraphicsResourceDesc());
}

SharedPtr<RenderTargetTexture> GraphicsDevice::createRenderTargetTexture(const RenderTargetTextureDesc& desc)
{
    return make_shared<RenderTargetTexture>(desc, getGraphicsResourceDesc());
}

SharedPtr<SpriteBatch> GraphicsDevice::createSpriteBatch(const SpriteBatchDesc& desc)
{
    return make_shared<SpriteBatch>(desc, getGraphicsResourceDesc());
}

SharedPtr<FontAtlas> GraphicsDevice::createFontAtlas(const FontAtlasDesc& desc)
{
    return make_shared<FontAtlas>(desc, getGraphicsResourceDesc());
}

void GraphicsDevice::clearCache()
{
    m_pipelineCache->clear();
    m_samplerCache->clear();
}

void GraphicsDevice::clearState()
{
    m_d3dContext->ClearState();
}

void GraphicsDevice::clearCommandList(DeviceContext& context)
{
    context.m_context->ClearState();
}

void GraphicsDevice::executeCommandList(DeviceContext& context)
{
    Microsoft::WRL::ComPtr<ID3D11CommandList> list;
    GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
        context.m_context->FinishCommandList(false, &list),
        "FinishCommandList failed."
    );
    m_d3dContext->ExecuteCommandList(list.Get(), false);
}

GraphicsResourceDesc GraphicsDevice::getGraphicsResourceDesc() noexcept
{
    return GraphicsResourceDesc{
        m_logger,
        shared_from_this(),
        *m_d3dDevice.Get(),
        *m_dxgiFactory.Get()
    };
}