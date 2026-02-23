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
    m_graphicsPipeline = m_graphicsDevice->createGraphicsPipelineState(GraphicsPipelineStateDesc{*vsSignature, *ps, PrimitiveTopology::Triangles});
    
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

void GraphicsEngine::clearPipeline()
{
    m_graphicsDevice->clearState();
}

void GraphicsEngine::render(SwapChain& swapChain)
{
    m_deviceContext->clearAndSetBackBuffer(swapChain, Vec4{0.27f, 0.39f, 0.55f, 1.0f});
    m_deviceContext->setGraphicsPipelineState(*m_graphicsPipeline);
    m_deviceContext->setViewport(swapChain.getSize());
    m_deviceContext->setVertexBuffer(*m_vertexBuffer);
    m_deviceContext->draw(m_vertexBuffer->getVertexListSize(), 0);

    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present();
}