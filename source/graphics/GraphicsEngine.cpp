#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/DeviceContext.h>
#include <graphics/utils/GraphicsMacros.h>
#include <resources/Mesh.h>
#include <resources/Texture.h>
#include <resources/Material.h>
#include <entity/Player.h>
#include <entity/Entity.h>
#include <entity/EntityManager.h>
#include <entity/components/Transform.h>
#include <entity/components/MeshRenderer.h>
#include <entity/components/Camera.h>
#include <game/World.h>

using namespace genesis;
using namespace std;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_unique<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_deviceContext = m_graphicsDevice->createDeviceContext();
    m_cameraBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(CameraData)});
    m_objectBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(ObjectData)});
    m_lightBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(LightData)});
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

    m_deviceContext->clearAndSetBackBuffer(swapChain, Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    m_deviceContext->setViewport(swapChain.getSize());

    CameraData cameraData = {
        camera->getViewMatrix(),
        camera->getProjectionMatrix(),
        cameraTransform->getPosition()
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData);
    m_deviceContext->setConstantBuffer(*m_cameraBuffer, CAMERA_CONSTANT_BUFFER_SLOT);

    LightData lightData = {
        Vec4{0.4f, -0.3f, 0.8f},
        Vec4{1.0f,  1.0f, 1.0f},
        100.0f
    };
    m_deviceContext->updateConstantBuffer(*m_lightBuffer, &lightData);
    m_deviceContext->setConstantBuffer(*m_lightBuffer, LIGHT_CONSTANT_BUFFER_SLOT);

    for (auto& [id, entity] : world.getEntityManager().getEntities()) {
        MeshRenderer* mesh = entity->getComponent<MeshRenderer>();
        Transform* transform = entity->getComponent<Transform>();

        if (!mesh || !transform) {
            continue;
        }

        ObjectData objectData{
            transform->getWorldMatrix()
        };
        m_deviceContext->updateConstantBuffer(*m_objectBuffer, &objectData);
        m_deviceContext->setConstantBuffer(*m_objectBuffer, OBJECT_CONSTANT_BUFFER_SLOT);

        Material* material = mesh->getMaterial();
        if (material->hasProperties()) {
            m_deviceContext->setConstantBuffer(material->getProperties(), MATERIAL_CONSTANT_BUFFER_SLOT);
        }

        auto& textures = material->getTextures();
        for (auto& binding : textures) {
            m_deviceContext->setTexture(binding.texture->getGraphicsTexture(), binding.slot);
        }

        m_deviceContext->setGraphicsPipelineState(material->getGraphicsPipelineState());
        m_deviceContext->setVertexBuffer(mesh->getMesh()->getVertexBuffer());
        m_deviceContext->setIndexBuffer(mesh->getMesh()->getIndexBuffer());
        m_deviceContext->drawIndexed(mesh->getMesh()->getIndexBuffer().getIndexCount());
    }

    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present();
}