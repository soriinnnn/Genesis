#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/EngineShaders.h>
#include <graphics/EngineStates.h>
#include <graphics/FrameBuffer.h>
#include <graphics/resources/DeviceContext.h>
#include <graphics/utils/GraphicsMacros.h>
#include <resources/Mesh.h>
#include <resources/Texture.h>
#include <resources/Material.h>
#include <resources/PostProcess.h>
#include <entity/Entity.h>
#include <entity/EntityManager.h>
#include <entity/components/LightComponent.h>
#include <entity/components/CameraComponent.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <ui/UIManager.h>

#define FULLSCREEN_TRIANGLE_VERTEX_COUNT 3

using namespace genesis;
using namespace std;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_shaders = make_unique<EngineShaders>(EngineShadersDesc{m_logger, *m_graphicsDevice});
    m_states = make_unique<EngineStates>(EngineStatesDesc{m_logger, *m_graphicsDevice});
    m_primaryBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_secondaryBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_deviceContext = m_graphicsDevice->createDeviceContext();
    m_spriteBatch = m_graphicsDevice->createSpriteBatch({*m_deviceContext});
    m_sceneBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(SceneData)});
    m_cameraBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(CameraData)});
    m_objectBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(ObjectData)});
    m_lightsBuffer = m_graphicsDevice->createStructuredBuffer({nullptr, sizeof(LightData), DEFAULT_MAX_LIGHTS});

    auto& m_fullscreenShader = m_shaders->getFullscreenTriangle();
    m_frameBufferPipeline = m_graphicsDevice->createGraphicsPipelineState({
        *m_fullscreenShader.vsBinary,
        *m_fullscreenShader.psBinary,
        *m_fullscreenShader.vsSignature,
        *m_fullscreenShader.psSignature
    });
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsContext GraphicsEngine::getGraphicsContext() noexcept
{
    return {*m_graphicsDevice, *m_shaders};
}

Rect GraphicsEngine::getRenderResolution() const noexcept
{
    return m_primaryBuffer->getSize();
}

void GraphicsEngine::setRenderResolution(uint32 width, uint32 height)
{
    m_primaryBuffer->resize(width, height);
    m_secondaryBuffer->resize(width, height);
}

void GraphicsEngine::clear(const Vec4& color)
{
    m_deviceContext->clearRenderTarget(m_primaryBuffer->getRenderTarget(), color);
    m_deviceContext->clearDepthStencil(m_primaryBuffer->getDepthStencil());
}

void GraphicsEngine::render(EntityManager& entities, Entity& camera, float deltaTime)
{
    CameraComponent* cameraComponent = camera.getComponent<CameraComponent>();
    if (!cameraComponent) {
        return;
    }

    m_deviceContext->setRenderTarget(m_primaryBuffer->getRenderTarget(), m_primaryBuffer->getDepthStencil());
    m_deviceContext->setViewport(m_primaryBuffer->getSize());

    CameraData cameraData = {
        cameraComponent->getViewMatrix(),
        cameraComponent->getProjectionMatrix(),
        Vec4{camera.getComponent<TransformComponent>()->getPosition()}
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData);
    m_deviceContext->setConstantBuffer(*m_cameraBuffer, CAMERA_CONSTANT_BUFFER_SLOT);

    uint32 lightCount = getLights(entities);
    SceneData sceneData = {
        deltaTime,
        lightCount
    };
    m_deviceContext->updateConstantBuffer(*m_sceneBuffer, &sceneData);
    m_deviceContext->setConstantBuffer(*m_sceneBuffer, SCENE_CONSTANT_BUFFER_SLOT);

    renderEntities(entities);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
}

void GraphicsEngine::render(DebugRenderer& debug, Entity& camera)
{
    CameraComponent* cameraComponent = camera.getComponent<CameraComponent>();
    if (!cameraComponent) {
        return;
    }

    m_deviceContext->setRenderTarget(m_primaryBuffer->getRenderTarget(), m_primaryBuffer->getDepthStencil());
    m_deviceContext->setViewport(m_primaryBuffer->getSize());

    CameraData cameraData = {
        cameraComponent->getViewMatrix(),
        cameraComponent->getProjectionMatrix(),
        Vec4{camera.getComponent<TransformComponent>()->getPosition()}
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData);
    m_deviceContext->setConstantBuffer(*m_cameraBuffer, CAMERA_CONSTANT_BUFFER_SLOT);

    debug.render(*m_deviceContext);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
}

void GraphicsEngine::render(UIManager& ui)
{
    m_deviceContext->setRenderTarget(m_primaryBuffer->getRenderTarget(), m_primaryBuffer->getDepthStencil());
    m_deviceContext->setViewport(m_primaryBuffer->getSize());

    try {
        m_spriteBatch->begin();
        ui.forEach([&](UIElement& element) {
            element.render(*m_spriteBatch);
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

void GraphicsEngine::present(SwapChain& swapChain, bool vsync)
{
    m_deviceContext->clearAndSetBackBuffer(swapChain, Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    m_deviceContext->setViewport(swapChain.getSize());
    m_deviceContext->setGraphicsPipelineState(*m_frameBufferPipeline);
    m_deviceContext->setTexture(m_primaryBuffer->getRenderTarget());
    m_deviceContext->setSamplerState(m_states->getPointClamp());
    m_deviceContext->draw(FULLSCREEN_TRIANGLE_VERTEX_COUNT);
    m_graphicsDevice->executeCommandList(*m_deviceContext);
    swapChain.present(vsync);
}

uint32 GraphicsEngine::getLights(EntityManager& entities)
{
    Vector<LightData> lights;
    lights.reserve(DEFAULT_MAX_LIGHTS);

    entities.forEachComponent<LightComponent>([&](LightComponent& lightComponent) {
        if (!lightComponent.isEnabled()) {
            return;
        }
        if (lights.size() > DEFAULT_MAX_LIGHTS) {
            return;
        }

        Entity& entity = lightComponent.getEntity();
        TransformComponent* transformComponent = entity.getComponent<TransformComponent>();
        if (!transformComponent) {
            return;
        }

        LightData data{};
        data.position = transformComponent->getPosition();
        data.direction = transformComponent->getForwardVector();
        data.color = lightComponent.getColor();
        data.radius = lightComponent.getRadius();
        data.intensity = lightComponent.getIntensity();
        data.type = static_cast<int>(lightComponent.getType());

        lights.push_back(data);
    });

    m_deviceContext->updateStructuredBuffer(*m_lightsBuffer, lights.data(), static_cast<uint32>(lights.size()) * sizeof(LightData));
    m_deviceContext->setStructuredBuffer(*m_lightsBuffer, LIGHT_DATA_TEXTURE_SLOT);

    return static_cast<uint32>(lights.size());
}

void GraphicsEngine::renderEntities(EntityManager& entities)
{
    entities.forEachComponent<MeshRendererComponent>([&](MeshRendererComponent& meshComponent) {
        Entity& entity = meshComponent.getEntity();
        TransformComponent* transformComponent = entity.getComponent<TransformComponent>();

        ObjectData objectData{
           transformComponent->getWorldMatrix()
        };
        m_deviceContext->updateConstantBuffer(*m_objectBuffer, &objectData);
        m_deviceContext->setConstantBuffer(*m_objectBuffer, OBJECT_CONSTANT_BUFFER_SLOT);

        Material* material = meshComponent.getMaterial();
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

        Mesh* mesh = meshComponent.getMesh();
        m_deviceContext->setVertexBuffer(mesh->getVertexBuffer());
        m_deviceContext->setIndexBuffer(mesh->getIndexBuffer());
        m_deviceContext->drawIndexed(mesh->getIndexBuffer().getIndexCount());
    });
}

void GraphicsEngine::applyPostProcess(PostProcess& effect, FrameBuffer& input, FrameBuffer& output)
{
    m_deviceContext->setRenderTarget(output.getRenderTarget(), output.getDepthStencil());
    m_deviceContext->setViewport(output.getSize());
    m_deviceContext->setGraphicsPipelineState(effect.getGraphicsPipelineState());
    m_deviceContext->setTexture(input.getRenderTarget());
    m_deviceContext->setSamplerState(m_states->getPointClamp());
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