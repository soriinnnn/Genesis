#include <graphics/GraphicsDevice.h>
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
}

GraphicsDevice::~GraphicsDevice() {}

SharedPtr<SwapChain> GraphicsDevice::createSwapChain(const SwapChainDesc& desc)
{
    return make_shared<SwapChain>(desc, getGraphicsResourceDesc());
}

SharedPtr<DeviceContext> GraphicsDevice::createDeviceContext()
{
    return make_shared<DeviceContext>(getGraphicsResourceDesc());
}

SharedPtr<ShaderBinary> GraphicsDevice::compileShader(const ShaderCompileDesc& desc)
{
    return make_shared<ShaderBinary>(desc, getGraphicsResourceDesc());
}

SharedPtr<GraphicsPipelineState> GraphicsDevice::createGraphicsPipelineState(const GraphicsPipelineStateDesc& desc)
{
    return make_shared<GraphicsPipelineState>(desc, getGraphicsResourceDesc());
}

SharedPtr<VertexBuffer> GraphicsDevice::createVertexBuffer(const VertexBufferDesc& desc)
{
    return make_shared<VertexBuffer>(desc, getGraphicsResourceDesc());
}

SharedPtr<VertexShaderSignature> GraphicsDevice::createVertexShaderSignature(const VertexShaderSignatureDesc& desc)
{
    return make_shared<VertexShaderSignature>(desc, getGraphicsResourceDesc());
}

SharedPtr<PixelShaderSignature> GraphicsDevice::createPixelShaderSignature(const PixelShaderSignatureDesc& desc)
{
    return make_shared<PixelShaderSignature>(desc, getGraphicsResourceDesc());
}

SharedPtr<ConstantBuffer> GraphicsDevice::createConstantBuffer(const ConstantBufferDesc& desc)
{
    return make_shared<ConstantBuffer>(desc, getGraphicsResourceDesc());
}

SharedPtr<IndexBuffer> GraphicsDevice::createIndexBuffer(const IndexBufferDesc& desc)
{
    return make_shared<IndexBuffer>(desc, getGraphicsResourceDesc());
}

SharedPtr<GraphicsTexture> GraphicsDevice::createGraphicsTexture(const GraphicsTextureDesc& desc)
{
    return make_shared<GraphicsTexture>(desc, getGraphicsResourceDesc());
}

SharedPtr<DepthBuffer> GraphicsDevice::createDepthBuffer(const DepthBufferDesc& desc)
{
    return make_shared<DepthBuffer>(desc, getGraphicsResourceDesc());
}

void GraphicsDevice::clearState()
{
    m_d3dContext->ClearState();
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
        *this, 
        *m_d3dDevice.Get(), 
        *m_dxgiFactory.Get()
    };
}
