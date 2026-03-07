#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/DeviceContext.h>

#include <misc/AssetManager.h>

using namespace genesis;
using namespace std;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_deviceContext = m_graphicsDevice->createDeviceContext();

    constexpr char vertexShaderFilePath[] = "demo/assets/shaders/basic/vs.hlsl";
    string shaderFileData = AssetManager::readFile(vertexShaderFilePath);
    if (shaderFileData.empty()) {
        GENESIS_LOG_THROW_ERROR("Failed to open shader file.");
    }
    SharedPtr<ShaderBinary> vs = m_graphicsDevice->compileShader(
        ShaderCompileDesc{
            vertexShaderFilePath, 
            shaderFileData.c_str(),
            shaderFileData.length(),
            "main",
            ShaderType::VertexShader
        }
    );

    constexpr char pixelShaderFilePath[] = "demo/assets/shaders/basic/ps.hlsl";
    shaderFileData = AssetManager::readFile(pixelShaderFilePath);
    if (shaderFileData.empty()) {
        GENESIS_LOG_THROW_ERROR("Failed to open shader file.");
    }
    SharedPtr<ShaderBinary> ps = m_graphicsDevice->compileShader(
        ShaderCompileDesc{
            pixelShaderFilePath,
            shaderFileData.c_str(),
            shaderFileData.length(),
            "main",
            ShaderType::PixelShader
        }
    );

    SharedPtr<VertexShaderSignature> vsSignature = m_graphicsDevice->createVertexShaderSignature(VertexShaderSignatureDesc{vs});
    m_graphicsPipeline = m_graphicsDevice->createGraphicsPipelineState(GraphicsPipelineStateDesc{*vsSignature, *ps, PrimitiveTopology::Triangles});
    
    const Vertex vertices[] = {
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0, 1}},
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1, 1}},
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 1, 1}},
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 1, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1, 1}},
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0, 1}},
        {{-0.5f, -0.5f,  0.5f}, {1, 0, 0, 1}}
    };
    m_vertexBuffer = m_graphicsDevice->createVertexBuffer(VertexBufferDesc{vertices, size(vertices), sizeof(Vertex)});
    
    const uint32 indices[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        1, 6, 5,
        5, 2, 1,
        7, 0, 3,
        3, 4, 7,
        3, 2, 5,
        5, 4, 3,
        7, 6, 1,
        1, 0, 7
    };
    m_indexBuffer = m_graphicsDevice->createIndexBuffer(IndexBufferDesc{indices, size(indices), IndexFormat::UnsignedInt32});

    m_constantBuffer = m_graphicsDevice->createConstantBuffer(ConstantBufferDesc{nullptr, sizeof(ConstantData)});
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsDevice& GraphicsEngine::getGraphicsDevice() noexcept
{
    return *m_graphicsDevice;
}

void GraphicsEngine::clearPipelineState()
{
    m_graphicsDevice->clearState();
}

void GraphicsEngine::render(SwapChain& swapChain, float deltaTime)   
{
    m_deltaTime = deltaTime;
    m_scale = 1.0f;
    
    Mat4 world = 
        Mat4::fromScale(Vec3{m_scale, m_scale, m_scale}) *
        Mat4::fromRotation(m_rot) *
        Mat4::fromTranslation(Vec3{m_pos.x, m_pos.y, 0.0f});

    Mat4 view = Mat4::identity();

    auto size = swapChain.getSize();
    float aspect = static_cast<float>(size.width()) / size.height();
    auto unitsPerScreenHeight = 2.0f;
    auto viewHeight = unitsPerScreenHeight;
    auto viewWidth = unitsPerScreenHeight * aspect;
    Mat4 projection = Mat4::orthographicLH(viewWidth, viewHeight, -10.0f, 10.0f);

    ConstantData m_data{
        world,
        view,
        projection
    };
    m_deviceContext->updateConstantBuffer(*m_constantBuffer, &m_data);

    // ---------------------------

    m_deviceContext->clearAndSetBackBuffer(swapChain, Vec4{0.27f, 0.39f, 0.55f, 1.0f});
    m_deviceContext->setGraphicsPipelineState(*m_graphicsPipeline);
    m_deviceContext->setViewport(swapChain.getSize());
    m_deviceContext->setVertexBuffer(*m_vertexBuffer);
    m_deviceContext->setConstantBuffer(*m_constantBuffer);
    //m_deviceContext->draw(m_vertexBuffer->getVertexCount());

    m_deviceContext->setIndexBuffer(*m_indexBuffer);
    m_deviceContext->drawIndexed(m_indexBuffer->getIndexCount());

    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present();
}

// PROVA

void genesis::GraphicsEngine::onKeyDown(Key key)
{
    if (key == Key::W) {
        m_rot.x += 0.707f * m_deltaTime;
    }
    else if (key == Key::S) {
        m_rot.x -= 0.707f * m_deltaTime;
    }
    else if (key == Key::A) {
        m_rot.y += 0.707f * m_deltaTime;
    }
    else if (key == Key::D) {
        m_rot.y -= 0.707f * m_deltaTime;
    }
}

void GraphicsEngine::onKeyUp(Key key)
{
}

void GraphicsEngine::onMouseMove(Point delta, Point pos)
{
    //GENESIS_LOG_INFO("Delta: x={}, y={}, Pos: x={}, y={}", delta.x, delta.y, pos.x, pos.y);
    m_rot.x += delta.x * m_deltaTime;
    m_rot.y += delta.y * m_deltaTime;
}

void GraphicsEngine::onMouseDown(MouseButton button, Point pos)
{
    //GENESIS_LOG_INFO("Button down: {}, Pos: x={}, y={}", (int)button, pos.x, pos.y);
}

void GraphicsEngine::onMouseUp(MouseButton button, Point pos)
{
    //GENESIS_LOG_INFO("Button up: {}, Pos: x={}, y={}", (int)button, pos.x, pos.y);
}
