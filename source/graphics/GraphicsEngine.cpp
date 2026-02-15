#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/DeviceContext.h>
#include <graphics/SwapChain.h>
#include <math/Vec3.h>

using namespace genesis;
using namespace std;

// --------------------------------------------------------------------------------

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_deviceContext = m_graphicsDevice->createDeviceContext();

    // XDDDDDDDDDDDDDDDDDDDDDDDDDD

    constexpr char shaderSourceCode[] = 
    R"(
        float4 VSmain(float3 pos: POSITION): SV_Position
        {
            return float4(pos.xyz, 1);
        }

        void PSmain()
        {
        }
    )";
    constexpr char shaderSouceName[] = "Basic";
    constexpr size_t shaderSourceCodeSize = std::size(shaderSourceCode);

    ShaderBinaryPtr vs = m_graphicsDevice->compileShader(
        ShaderCompileDesc{
            shaderSouceName, 
            shaderSourceCode, 
            shaderSourceCodeSize, 
            "VSmain", 
            ShaderType::VertexShader
        }
    );

    ShaderBinaryPtr ps = m_graphicsDevice->compileShader(
        ShaderCompileDesc{
            shaderSouceName,
            shaderSourceCode,
            shaderSourceCodeSize,
            "PSmain",
            ShaderType::PixelShader
        }
    );

    m_graphicsPipeline = m_graphicsDevice->createGraphicsPipelineState(GraphicsPipelineStateDesc{*vs, *ps});
    
    const Vec3 vertexList[] = {
        {-0.5f, -0.5f, 0.0f},
        {0.0f, 0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f}
    };
    m_vertexBuffer = m_graphicsDevice->createVertexBuffer(VertexBufferDesc{vertexList, size(vertexList), sizeof(Vec3)});
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsDevice& GraphicsEngine::getGraphicsDevice() noexcept
{
    return *m_graphicsDevice;
}

void GraphicsEngine::render(SwapChain& swapChain)
{
    auto& device = *m_graphicsDevice;
    auto& context = *m_deviceContext;

    context.clearAndSetBackBuffer(swapChain, Vec4{1, 1, 1, 1});
    context.setGraphicsPipelineState(*m_graphicsPipeline);
    context.setViewportSize(swapChain.getSize());
    context.setVertexBuffer(*m_vertexBuffer);

    device.executeCommandList(context);
    swapChain.present();
}