#include <graphics/GraphicsDevice.h>
#include <graphics/DeviceContext.h>
#include <graphics/SwapChain.h>
#include <graphics/GraphicsLogUtils.h>
#include <graphics/ShaderBinary.h>
#include <graphics/GraphicsPipelineState.h>
#include <graphics/VertexBuffer.h>
#include <graphics/VertexShaderSignature.h>

using namespace genesis;
using namespace std;

GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) : Base(desc.base)
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

SwapChainPtr GraphicsDevice::createSwapChain(const SwapChainDesc& desc)
{
    return make_shared<SwapChain>(desc, getGraphicsResourceDesc());
}

DeviceContextPtr GraphicsDevice::createDeviceContext()
{
    return make_shared<DeviceContext>(getGraphicsResourceDesc());
}

ShaderBinaryPtr GraphicsDevice::compileShader(const ShaderCompileDesc& desc)
{
    return make_shared<ShaderBinary>(desc, getGraphicsResourceDesc());
}

GraphicsPipelineStatePtr GraphicsDevice::createGraphicsPipelineState(const GraphicsPipelineStateDesc& desc)
{
    return make_shared<GraphicsPipelineState>(desc, getGraphicsResourceDesc());
}

VertexBufferPtr GraphicsDevice::createVertexBuffer(const VertexBufferDesc& desc)
{
    return make_shared<VertexBuffer>(desc, getGraphicsResourceDesc());
}

VertexShaderSignaturePtr GraphicsDevice::createVertexShaderSignature(const VertexShaderSignatureDesc& desc)
{
    return make_shared<VertexShaderSignature>(desc, getGraphicsResourceDesc());
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

GraphicsResourceDesc GraphicsDevice::getGraphicsResourceDesc() const noexcept
{
    return GraphicsResourceDesc{
        BaseDesc{m_logger},
        shared_from_this(), 
        *m_d3dDevice.Get(), 
        *m_dxgiFactory.Get()
    };
}
