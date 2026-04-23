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
#include <entity/Entity.h>
#include <entity/components/Light.h>
#include <entity/components/Camera.h>
#include <entity/components/Transform.h>
#include <entity/components/MeshRenderer.h>
#include <ui/UIManager.h>
#include <game/World.h>

#define FULLSCREEN_TRIANGLE_VERTEX_COUNT 3

using namespace genesis;
using namespace std;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_engineShaders = make_unique<EngineShaders>(EngineShadersDesc{m_logger, *m_graphicsDevice});
    m_primaryBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_secondaryBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_deviceContext = m_graphicsDevice->createDeviceContext();
    m_spriteBatch = m_graphicsDevice->createSpriteBatch({*m_deviceContext});
    m_sceneBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(SceneData)});
    m_cameraBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(CameraData)});
    m_objectBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(ObjectData)});
    m_lightsBuffer = m_graphicsDevice->createStructuredBuffer({nullptr, sizeof(LightData), DEFAULT_MAX_LIGHTS});
    m_pointClampSampler = m_graphicsDevice->createSamplerState({
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

void GraphicsEngine::clear(Vec4 color)
{
    m_deviceContext->clearRenderTarget(m_primaryBuffer->getRenderTarget(), color);
    m_deviceContext->clearDepthStencil(m_primaryBuffer->getDepthStencil());
}

void GraphicsEngine::render(World& world, float deltaTime)
{
    Entity* camera = world.getCamera();
    if (!camera) {
        return;
    }

    m_deviceContext->setRenderTarget(m_primaryBuffer->getRenderTarget(), m_primaryBuffer->getDepthStencil());
    m_deviceContext->setViewport(m_primaryBuffer->getSize());

    Camera* cameraComponent = camera->getComponent<Camera>();
    CameraData cameraData = {
        cameraComponent->getViewMatrix(),
        cameraComponent->getProjectionMatrix(),
        camera->getComponent<Transform>()->getPosition()
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData);
    m_deviceContext->setConstantBuffer(*m_cameraBuffer, CAMERA_CONSTANT_BUFFER_SLOT);

    Vector<LightData> lights;
    lights.reserve(DEFAULT_MAX_LIGHTS);
    world.forEach([&](Entity& entity) {
        Light* lightComponent = entity.getComponent<Light>();
        
        if (!lightComponent || !lightComponent->isEnabled()) {
            return;
        }
        if (lights.size() > DEFAULT_MAX_LIGHTS) {
            return;
        }
        
        Transform* transformComponent = entity.getComponent<Transform>();
        if (!transformComponent) {
            return;
        }

        LightData data{};
        data.type = static_cast<int>(lightComponent->getType());
        data.color = lightComponent->getColor();
        data.intensity = lightComponent->getIntensity();
        data.radius = lightComponent->getRadius();
        data.direction = transformComponent->getForwardVector();
        data.position = transformComponent->getPosition();

        lights.push_back(data);
    });

    m_deviceContext->updateStructuredBuffer(*m_lightsBuffer, lights.data(), static_cast<uint32>(lights.size()) * sizeof(LightData));
    m_deviceContext->setStructuredBuffer(*m_lightsBuffer, LIGHT_DATA_TEXTURE_SLOT);

    SceneData sceneData = {
        deltaTime,
        static_cast<uint32>(lights.size())
    };
    m_deviceContext->updateConstantBuffer(*m_sceneBuffer, &sceneData);
    m_deviceContext->setConstantBuffer(*m_sceneBuffer, SCENE_CONSTANT_BUFFER_SLOT);

    renderEntities(world);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
}

void GraphicsEngine::render(UIManager& ui)
{
    m_deviceContext->setRenderTarget(m_primaryBuffer->getRenderTarget(), m_primaryBuffer->getDepthStencil());
    m_deviceContext->setViewport(m_primaryBuffer->getSize());

    try {
        m_spriteBatch->begin();
        ui.forEach([&](UIElement& element) {
            if (element.isVisible()) {
                element.render(*m_spriteBatch);
            }
        });
        m_spriteBatch->end();
    }
    catch (const std::exception& e) {
        GENESIS_LOG_THROW_ERROR("UI rendering failed.\nDetails: {}", e.what());
    }
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
    m_deviceContext->setSamplerState(*m_pointClampSampler);
    m_deviceContext->draw(FULLSCREEN_TRIANGLE_VERTEX_COUNT);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present();
}

void GraphicsEngine::renderEntities(World& world)
{
    world.forEach([&](Entity& entity) {
        MeshRenderer* mesh = entity.getComponent<MeshRenderer>();
        Transform* transform = entity.getComponent<Transform>();

        if (!mesh || !transform) {
            return;
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
    });
}

void GraphicsEngine::applyPostProcess(PostProcess& effect, FrameBuffer& input, FrameBuffer& output)
{
    m_deviceContext->setRenderTarget(output.getRenderTarget(), output.getDepthStencil());
    m_deviceContext->setViewport(output.getSize());
    m_deviceContext->setGraphicsPipelineState(effect.getGraphicsPipelineState());
    m_deviceContext->setTexture(input.getRenderTarget());
    m_deviceContext->setSamplerState(*m_pointClampSampler);
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