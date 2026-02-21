#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/DeviceContext.h>
#include <graphics/SwapChain.h>
#include <graphics/VertexBuffer.h>
#include <math/Vec3.h>
#include <fstream>

using namespace genesis;
using namespace std;

inline constexpr char vertexShaderEntryPoint[] = "VSmain";
inline constexpr char pixelShaderEntryPoint[] = "PSmain";

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_deviceContext = m_graphicsDevice->createDeviceContext();

    constexpr char shaderFilePath[] = "assets/shaders/basic.hlsl";
    ifstream shaderStream(shaderFilePath);
    if (!shaderStream) {
        GENESIS_LOG_THROW_ERROR("Failed to open shader file.");
    }

    string shaderFileData{
        istreambuf_iterator<char>(shaderStream),
        istreambuf_iterator<char>()
    };

    const char* shaderSourceCode = shaderFileData.c_str();
    size_t shaderSourceCodeSize = shaderFileData.length();

    ShaderBinaryPtr vs = m_graphicsDevice->compileShader(
        ShaderCompileDesc{
            shaderFilePath, 
            shaderSourceCode, 
            shaderSourceCodeSize, 
            vertexShaderEntryPoint,
            ShaderType::VertexShader
        }
    );

    ShaderBinaryPtr ps = m_graphicsDevice->compileShader(
        ShaderCompileDesc{
            shaderFilePath,
            shaderSourceCode,
            shaderSourceCodeSize,
            pixelShaderEntryPoint,
            ShaderType::PixelShader
        }
    );

    VertexShaderSignaturePtr vsSignature = m_graphicsDevice->createVertexShaderSignature(VertexShaderSignatureDesc{vs});
    m_graphicsPipeline = m_graphicsDevice->createGraphicsPipelineState(GraphicsPipelineStateDesc{*vsSignature, *ps});
    
    const Vertex vertexList[] = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f }, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},

        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}}
    };
    m_vertexBuffer = m_graphicsDevice->createVertexBuffer(VertexBufferDesc{vertexList, size(vertexList), sizeof(Vertex)});
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsDevice& GraphicsEngine::getGraphicsDevice() noexcept
{
    return *m_graphicsDevice;
}

void GraphicsEngine::render(SwapChain& swapChain)
{
    GraphicsDevice& device = *m_graphicsDevice;
    DeviceContext& context = *m_deviceContext;

    context.clearAndSetBackBuffer(swapChain, Vec4{0.27f, 0.39f, 0.55f, 1.0f});
    context.setGraphicsPipelineState(*m_graphicsPipeline);
    context.setViewportSize(swapChain.getSize());
    context.setVertexBuffer(*m_vertexBuffer);
    context.drawTriangleList(m_vertexBuffer->getVertexListSize(), 0);

    device.executeCommandList(context);
    swapChain.present();
}