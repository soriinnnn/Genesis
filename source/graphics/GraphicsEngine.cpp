#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/DeviceContext.h>
#include <resources/Mesh.h>
#include <resources/Shader.h>
#include <resources/Texture.h>
#include <resources/Material.h>
#include <entity/Entity.h>
#include <entity/Player.h>
#include <entity/EntityManager.h>
#include <entity/components/Transform.h>
#include <entity/components/Camera.h>
#include <entity/components/MeshRenderer.h>
#include <game/World.h>

#define CAMERA_CONSTANT_BUFFER_NAME "CameraData"
#define OBJECT_CONSTANT_BUFFER_NAME "ObjectData"

using namespace genesis;
using namespace std;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_unique<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_deviceContext = m_graphicsDevice->createDeviceContext();
    m_cameraBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(CameraData)});
    m_objectBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(ObjectData)});
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsDevice& GraphicsEngine::getGraphicsDevice() noexcept
{
    return *m_graphicsDevice;
}

void GraphicsEngine::render(World& world, SwapChain& swapChain, float deltaTime)
{
    Camera* camera = world.getPlayer()->getComponent<Camera>();
    Transform* cameraTransform = world.getPlayer()->getComponent<Transform>();

    if (!camera) {
        return;
    }

    m_deviceContext->clearAndSetBackBuffer(swapChain, Vec4{0.27f, 0.39f, 0.55f, 1.0f});
    m_deviceContext->setViewport(swapChain.getSize());

    CameraData cameraData = {
        camera->getViewMatrix(),
        camera->getProjectionMatrix(),
        cameraTransform->getPosition(),
        0.0f,
        Vec4{0.4f, -0.3f, 0.8f, 0.0f}
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData);

    for (auto& [id, entity] : world.getEntityManager().getEntities()) {
        MeshRenderer* mesh = entity->getComponent<MeshRenderer>();
        Transform* transform = entity->getComponent<Transform>();

        if (!mesh || !transform) {
            continue;
        }

        Material* material = mesh->getMaterial();
        auto& vsSignature = material->getVertexShader().getSignature();
        auto& psSignature = material->getPixelShader().getSignature();

        m_deviceContext->setConstantBuffer(*m_cameraBuffer, ShaderType::VertexShader, vsSignature.getConstantBufferSlot(CAMERA_CONSTANT_BUFFER_NAME));
        if (psSignature.hasConstantBuffer(CAMERA_CONSTANT_BUFFER_NAME)) {
            m_deviceContext->setConstantBuffer(*m_cameraBuffer, ShaderType::PixelShader, psSignature.getConstantBufferSlot(CAMERA_CONSTANT_BUFFER_NAME));
        }

        ObjectData objectData{
            transform->getWorldMatrix()
        };
        m_deviceContext->updateConstantBuffer(*m_objectBuffer, &objectData);

        m_deviceContext->setConstantBuffer(*m_objectBuffer, ShaderType::VertexShader, vsSignature.getConstantBufferSlot(OBJECT_CONSTANT_BUFFER_NAME));
        if (psSignature.hasConstantBuffer(OBJECT_CONSTANT_BUFFER_NAME)) {
            m_deviceContext->setConstantBuffer(*m_objectBuffer, ShaderType::PixelShader, psSignature.getConstantBufferSlot(OBJECT_CONSTANT_BUFFER_NAME));
        }

        if (material->hasProperties()) {
            m_deviceContext->setConstantBuffer(material->getProperties(), ShaderType::PixelShader, material->getPropertiesSlot());
        }

        auto& textures = material->getTextures();
        for (uint32 i = 0; i < textures.size(); i++) {
            m_deviceContext->setTexture(textures[i]->getGraphicsTexture(), i);
        }

        m_deviceContext->setGraphicsPipelineState(material->getGraphicsPipelineState());
        m_deviceContext->setVertexBuffer(mesh->getMesh()->getVertexBuffer());
        m_deviceContext->setIndexBuffer(mesh->getMesh()->getIndexBuffer());
        m_deviceContext->drawIndexed(mesh->getMesh()->getIndexBuffer().getIndexCount());
    }

    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present();
}