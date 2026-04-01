#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/EngineShaders.h>
#include <graphics/FrameBuffer.h>
#include <graphics/resources/DeviceContext.h>
#include <graphics/utils/GraphicsMacros.h>
#include <resources/Mesh.h>
#include <resources/Texture.h>
#include <resources/Material.h>
#include <resources/PostProcess.h>
#include <entity/Player.h>
#include <entity/Entity.h>
#include <entity/EntityManager.h>
#include <entity/components/Camera.h>
#include <entity/components/Transform.h>
#include <entity/components/MeshRenderer.h>
#include <game/World.h>

#define FULLSCREEN_TRIANGLE_VERTEX_COUNT 3

using namespace genesis;
using namespace std;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_unique<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_engineShaders = make_unique<EngineShaders>(EngineShadersDesc{m_logger, *m_graphicsDevice});
    m_primaryBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_secondaryBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_deviceContext = m_graphicsDevice->createDeviceContext();
    m_cameraBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(CameraData)});
    m_objectBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(ObjectData)});
    m_lightBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(LightData)});
    m_pointSampler = m_graphicsDevice->createSamplerState({
        TextureFilter::Linear,
        TextureAddressMode::Clamp,
        TextureAddressMode::Clamp,
        TextureAddressMode::Clamp
    });
    m_framePipeline = m_graphicsDevice->createGraphicsPipelineState({
        m_engineShaders->getFullscreenVSBinary(),
        m_engineShaders->getFullscreenPSBinary(),
        m_engineShaders->getFullscreenVSSignature(),
        m_engineShaders->getFullscreenPSSignature()
    });
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsContext GraphicsEngine::getGraphicsContext() noexcept
{
    return {*m_graphicsDevice, *m_engineShaders};
}

void GraphicsEngine::resizeFrameBuffers(uint32 width, uint32 height)
{
    m_primaryBuffer->resize(width, height);
    m_secondaryBuffer->resize(width, height);
}

void GraphicsEngine::render(World& world)
{
    Camera* camera = world.getPlayer()->getComponent<Camera>();
    Transform* cameraTransform = world.getPlayer()->getComponent<Transform>();

    if (!camera) {
        return;
    }

    m_deviceContext->clearRenderTarget(m_primaryBuffer->getRenderTarget(), Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    m_deviceContext->clearDepthStencil(m_primaryBuffer->getDepthStencil());
    m_deviceContext->setRenderTarget(m_primaryBuffer->getRenderTarget(), m_primaryBuffer->getDepthStencil());
    m_deviceContext->setViewport(m_primaryBuffer->getSize());

    CameraData cameraData = {
        camera->getViewMatrix(),
        camera->getProjectionMatrix(),
        cameraTransform->getPosition()
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData);
    m_deviceContext->setConstantBuffer(*m_cameraBuffer, CAMERA_CONSTANT_BUFFER_SLOT);

    LightData lightData = {
        Vec4{-0.4f, -0.2f, 0.8f},
        Vec4{1.0f,  1.0f, 1.0f},
        100.0f
    };
    m_deviceContext->updateConstantBuffer(*m_lightBuffer, &lightData);
    m_deviceContext->setConstantBuffer(*m_lightBuffer, LIGHT_CONSTANT_BUFFER_SLOT);

    renderEntities(world);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
}

void GraphicsEngine::render(World& world, SwapChain& swapChain)
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
        Vec4{-0.4f, -0.2f, 0.8f},
        Vec4{1.0f,  1.0f, 1.0f},
        100.0f
    };
    m_deviceContext->updateConstantBuffer(*m_lightBuffer, &lightData);
    m_deviceContext->setConstantBuffer(*m_lightBuffer, LIGHT_CONSTANT_BUFFER_SLOT);

    renderEntities(world);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present();
}

void GraphicsEngine::postProcess(PostProcess& effect)
{
    applyPostProcess(effect, *m_primaryBuffer, *m_secondaryBuffer);
    std::swap(m_primaryBuffer, m_secondaryBuffer);
}

void GraphicsEngine::present(SwapChain& swapChain)
{
    m_deviceContext->clearAndSetBackBuffer(swapChain, Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    m_deviceContext->setViewport(swapChain.getSize());
    m_deviceContext->setGraphicsPipelineState(*m_framePipeline);
    m_deviceContext->setTexture(m_primaryBuffer->getRenderTarget());
    m_deviceContext->setSamplerState(*m_pointSampler);
    m_deviceContext->draw(FULLSCREEN_TRIANGLE_VERTEX_COUNT);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present();
}

void GraphicsEngine::renderEntities(World& world)
{
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
        for (auto& binding : material->getTextures()) {
            m_deviceContext->setTexture(binding.texture->getImageTexture(), binding.slot);
        }
        for (auto& binding : material->getSamplers()) {
            m_deviceContext->setSamplerState(*binding.sampler, binding.slot);
        }

        m_deviceContext->setGraphicsPipelineState(material->getGraphicsPipelineState());
        m_deviceContext->setVertexBuffer(mesh->getMesh()->getVertexBuffer());
        m_deviceContext->setIndexBuffer(mesh->getMesh()->getIndexBuffer());
        m_deviceContext->drawIndexed(mesh->getMesh()->getIndexBuffer().getIndexCount());
    }
}

void GraphicsEngine::applyPostProcess(PostProcess& effect, FrameBuffer& input, FrameBuffer& output)
{
    m_deviceContext->setRenderTarget(output.getRenderTarget(), output.getDepthStencil());
    m_deviceContext->setViewport(output.getSize());
    m_deviceContext->setGraphicsPipelineState(effect.getGraphicsPipelineState());
    m_deviceContext->setTexture(input.getRenderTarget());
    m_deviceContext->setSamplerState(*m_pointSampler);
    if (effect.hasProperties()) {
        if (effect.isDirty()) {
            m_deviceContext->updateConstantBuffer(effect.getProperties(), effect.getData());
            effect.clearDirty();
        }
        m_deviceContext->setConstantBuffer(effect.getProperties(), POST_PROCESSING_EFFECT_CONSTANT_BUFFER_SLOT);
    }
    m_deviceContext->draw(FULLSCREEN_TRIANGLE_VERTEX_COUNT);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
}