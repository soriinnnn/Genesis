#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/DeviceContext.h>
#include <resources/VertexShader.h>
#include <resources/PixelShader.h>
#include <resources/Texture.h>

#include <resources/ResourceManager.h>

using namespace genesis;
using namespace std;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_unique<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_deviceContext = m_graphicsDevice->createDeviceContext();
    
    const Vertex vertices[] = {
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0, 1}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0, 1}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1, 1}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 1, 1}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 1, 1}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1, 1}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0, 1}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1, 0, 0, 1}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0, 1}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0, 1}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1, 1}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1, 1}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1, 0, 0, 1}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0, 1}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 1, 1}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 1, 1}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {1, 0, 0, 1}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0, 1}, {0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0, 1}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0, 1}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 1, 1}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1, 1}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1, 1}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 1, 1}, {1.0f, 1.0f}}
    };
    m_vertexBuffer = m_graphicsDevice->createVertexBuffer(VertexBufferDesc{vertices, size(vertices), sizeof(Vertex)});

    const uint32 indices[] = {
         0,  1,  2, 
         2,  3,  0,
         4,  5,  6, 
         6,  7,  4,
         8,  9, 10,
        10, 11,  8,
        12, 13, 14,
        14, 15, 12,
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
    };
    m_indexBuffer = m_graphicsDevice->createIndexBuffer(IndexBufferDesc{indices, size(indices), IndexFormat::UnsignedInt32});
    m_constantBuffer = m_graphicsDevice->createConstantBuffer(ConstantBufferDesc{nullptr, sizeof(ConstantData)});

    m_pos = Vec3{0, 0, -2};
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsDevice& GraphicsEngine::getGraphicsDevice() noexcept
{
    return *m_graphicsDevice;
}

void GraphicsEngine::render(Texture& tex, SwapChain& swapChain, float deltaTime)
{
    m_deltaTime = deltaTime;
    m_scale = 1.0f;

    Mat4 world = Mat4::identity();

    float yaw = m_rot.y;
    float pitch = m_rot.x;
    Vec3 direction{
        std::cos(pitch) * std::sin(yaw),
        -std::sin(pitch),
        std::cos(pitch) * std::cos(yaw)
    };
    Mat4 view = Mat4::lookAtLH(m_pos, m_pos + direction, Vec3{0, 1, 0});

    auto size = swapChain.getSize();
    float aspect = static_cast<float>(size.width()) / size.height();
    auto unitsPerScreenHeight = 2.0f;
    auto viewHeight = unitsPerScreenHeight;
    auto viewWidth = unitsPerScreenHeight * aspect;
    //Mat4 projection = Mat4::orthographicLH(viewWidth, viewHeight, -10.0f, 10.0f);
    Mat4 projection = Mat4::perspectiveLH(1.57f, aspect, 0.1f, 100.0f);

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
    m_deviceContext->setTexture(tex.getGraphicsTexture());


    m_deviceContext->setIndexBuffer(*m_indexBuffer);
    m_deviceContext->drawIndexed(m_indexBuffer->getIndexCount());

    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present();
}

// PROVA

void genesis::GraphicsEngine::onKeyDown(Key key)
{
    Vec3 forward{
        std::cos(m_rot.x) * std::sin(m_rot.y),
        -std::sin(m_rot.x),
        std::cos(m_rot.x) * std::cos(m_rot.y)
    };

    Vec3 right = Vec3::normalize(Vec3::cross(Vec3{0, 1, 0}, forward));

    if (key == Key::W) {
        m_pos += forward * 1.0f * m_deltaTime;
    }
    if (key == Key::S) {
        m_pos -= forward * 1.0f * m_deltaTime;
    }
    if (key == Key::D) {
        m_pos += right * 1.0f * m_deltaTime;
    }
    if (key == Key::A) {
        m_pos -= right * 1.0f * m_deltaTime;
    }
}

void GraphicsEngine::onKeyUp(Key key)
{
}

void GraphicsEngine::onMouseMove(Point delta, Point pos)
{
    //GENESIS_LOG_INFO("Delta: x={}, y={}, Pos: x={}, y={}", delta.x, delta.y, pos.x, pos.y);
    m_rot.x += delta.y * m_deltaTime;
    m_rot.y += delta.x * m_deltaTime;

    const float maxPitch = 1.553f;
    if (m_rot.x > maxPitch)  m_rot.x = maxPitch;
    if (m_rot.x < -maxPitch) m_rot.x = -maxPitch;
}

void GraphicsEngine::onMouseDown(MouseButton button, Point pos)
{
    //GENESIS_LOG_INFO("Button down: {}, Pos: x={}, y={}", (int)button, pos.x, pos.y);
}

void GraphicsEngine::onMouseUp(MouseButton button, Point pos)
{
    //GENESIS_LOG_INFO("Button up: {}, Pos: x={}, y={}", (int)button, pos.x, pos.y);
}

void GraphicsEngine::setPipeline(ResourceManager& resourceManager) 
{
    SharedPtr<VertexShader> vs = resourceManager.createVertexShader("demo/assets/shaders/basic/vs.hlsl", "main");
    SharedPtr<PixelShader> ps = resourceManager.createPixelShader("demo/assets/shaders/basic/ps.hlsl", "main");
    m_graphicsPipeline = m_graphicsDevice->createGraphicsPipelineState(GraphicsPipelineStateDesc{vs->getVertexShaderSignature(), ps->getShaderBinary(), PrimitiveTopology::Triangles});
}