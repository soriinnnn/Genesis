#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/DeviceContext.h>
#include <resources/VertexShader.h>
#include <resources/PixelShader.h>
#include <resources/Texture.h>
#include <resources/Mesh.h>
#include <entity/Entity.h>
#include <entity/components/Transform.h>
#include <entity/components/Camera.h>
#include <entity/components/MeshRenderer.h>

using namespace genesis;
using namespace std;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_unique<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_deviceContext = m_graphicsDevice->createDeviceContext();
 
    m_constantBuffer = m_graphicsDevice->createConstantBuffer(ConstantBufferDesc{nullptr, sizeof(ConstantData)});
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsDevice& GraphicsEngine::getGraphicsDevice() noexcept
{
    return *m_graphicsDevice;
}

void GraphicsEngine::render(Entity& entity, SwapChain& swapChain, float deltaTime)
{
    Transform* transform = entity.getComponent<Transform>();
    Camera* camera = entity.getComponent<Camera>();
    MeshRenderer* mesh = entity.getComponent<MeshRenderer>();

    if (camera) {
        ConstantData m_data{
            transform->getWorldMatrix(),
            camera->getViewMatrix(),
            camera->getProjectionMatrix(),
            {0.4f, -0.3f, 0.8f},
            transform->getPosition()
        };
        m_deviceContext->updateConstantBuffer(*m_constantBuffer, &m_data);
    }
    
    if (mesh) {
        m_deviceContext->clearAndSetBackBuffer(swapChain, Vec4{0.27f, 0.39f, 0.55f, 1.0f});
        m_deviceContext->setViewport(swapChain.getSize());
        m_deviceContext->setConstantBuffer(*m_constantBuffer);

        m_deviceContext->setGraphicsPipelineState(*(mesh->getGraphicsPipelineState()));
        m_deviceContext->setVertexBuffer(mesh->getMesh()->getVertexBuffer());
        m_deviceContext->setIndexBuffer(mesh->getMesh()->getIndexBuffer());
        m_deviceContext->setTexture(mesh->getTexture()->getGraphicsTexture());
        m_deviceContext->drawIndexed(mesh->getMesh()->getIndexBuffer().getIndexCount());

        m_graphicsDevice->executeCommandList(*m_deviceContext);

        swapChain.present();
    }
}