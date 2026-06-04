#include <graphics/GraphicsEngine.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/EngineShaders.h>
#include <graphics/EngineStates.h>
#include <graphics/EngineMeshes.h>
#include <graphics/FrameBuffer.h>
#include <graphics/resources/DeviceContext.h>
#include <graphics/utils/GraphicsMacros.h>
#include <graphics/utils/GraphicsUtils.h>
#include <resources/Mesh.h>
#include <resources/Texture.h>
#include <resources/Material.h>
#include <resources/PostProcess.h>
#include <resources/SkyBox.h>
#include <entity/Entity.h>
#include <entity/EntityManager.h>
#include <entity/components/LightComponent.h>
#include <entity/components/CameraComponent.h>
#include <entity/components/TransformComponent.h>
#include <entity/components/MeshRendererComponent.h>
#include <display/Display.h>
#include <ui/UIManager.h>

#define FULLSCREEN_TRIANGLE_VERTEX_COUNT 3

using namespace genesis;
using namespace std;
using namespace graphicsUtils;

GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
    m_graphicsDevice = make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});
    m_shaders = make_unique<EngineShaders>(EngineShadersDesc{m_logger, *m_graphicsDevice});
    m_states = make_unique<EngineStates>(EngineStatesDesc{m_logger, *m_graphicsDevice, *m_shaders});
    m_meshes = make_unique<EngineMeshes>(EngineMeshesDesc{m_logger, *m_graphicsDevice});
    m_primaryBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_secondaryBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_msaaBuffer = make_unique<FrameBuffer>(FrameBufferDesc{m_logger, desc.buffersSize, *m_graphicsDevice});
    m_deviceContext = m_graphicsDevice->createDeviceContext();
    m_spriteBatch = m_graphicsDevice->createSpriteBatch({*m_deviceContext});
    m_sceneBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(SceneData)});
    m_cameraBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(CameraData)});
    m_objectBuffer = m_graphicsDevice->createConstantBuffer({nullptr, sizeof(ObjectData)});
    m_lightsBuffer = m_graphicsDevice->createStructuredBuffer({nullptr, sizeof(LightData), DEFAULT_MAX_LIGHTS});
    m_sceneRasterizer = &m_states->rasterizerSolid();
    m_sceneSampler = &m_states->trilinearWrap();
    m_sceneTarget = m_primaryBuffer.get();
    m_antiAliasing = AntiAliasing::None;
    m_textureFiltering = TextureFiltering::Trilinear;
    m_msaaDirty = false;
}

GraphicsEngine::~GraphicsEngine() {}

GraphicsContext GraphicsEngine::getGraphicsContext() noexcept
{
    return {*m_graphicsDevice, *m_shaders, *m_states};
}

Rect GraphicsEngine::getRenderResolution() const noexcept
{
    return m_primaryBuffer->getSize();
}

AntiAliasing GraphicsEngine::getAntiAliasing() const noexcept
{
    return m_antiAliasing;
}

TextureFiltering GraphicsEngine::getTextureFiltering() const noexcept
{
    return m_textureFiltering;
}

void GraphicsEngine::setRenderResolution(const Rect& resolution)
{
    m_primaryBuffer->setSize(resolution);
    m_secondaryBuffer->setSize(resolution);
    m_msaaBuffer->setSize(resolution);
}

void GraphicsEngine::setAntiAliasing(AntiAliasing mode)
{
    if (m_antiAliasing == mode) {
        return;
    }
    if (isHardwareMSAA(mode)) {
        m_sceneRasterizer = &m_states->rasterizerSolidMSAA();
        m_sceneTarget = m_msaaBuffer.get();
    }
    else {
        m_sceneRasterizer = &m_states->rasterizerSolid();
        m_sceneTarget = m_primaryBuffer.get();
    }
    m_msaaBuffer->setSampleCount(getMSAASampleCount(mode));
    m_antiAliasing = mode;
}

void GraphicsEngine::setTextureFiltering(TextureFiltering filter)
{
    if (m_textureFiltering == filter) {
        return;
    }
    switch (filter) {
        case TextureFiltering::Point: {
            m_sceneSampler = &m_states->pointWrap();
            break;
        }
        case TextureFiltering::Bilinear: {
            m_sceneSampler = &m_states->bilinearWrap();
            break;
        }
        case TextureFiltering::Trilinear: {
            m_sceneSampler = &m_states->trilinearWrap();
            break;
        }
        case TextureFiltering::Anisotropic_4X: {
            m_sceneSampler = &m_states->anisotropicWrap(getMaxAnisotropy(filter));
            break;
        }
        case TextureFiltering::Anisotropic_8X: {
            m_sceneSampler = &m_states->anisotropicWrap(getMaxAnisotropy(filter));
            break;
        }
        case TextureFiltering::Anisotropic_16X: {
            m_sceneSampler = &m_states->anisotropicWrap(getMaxAnisotropy(filter));
            break;
        }
    }
    m_textureFiltering = filter;
}

void GraphicsEngine::clear(const Vec4& color)
{
    m_deviceContext->clearRenderTarget(m_primaryBuffer->getRenderTarget(), color);
    m_deviceContext->clearDepthStencil(m_primaryBuffer->getDepthStencil());
    if (isHardwareMSAA(m_antiAliasing)) {
        m_deviceContext->clearRenderTarget(m_msaaBuffer->getRenderTarget(), color);
        m_deviceContext->clearDepthStencil(m_msaaBuffer->getDepthStencil());
    }
}

void GraphicsEngine::render(EntityManager& entities, Entity& camera, float deltaTime)
{
    CameraComponent* cameraComponent = camera.getComponent<CameraComponent>();
    if (!cameraComponent) {
        return;
    }

    m_deviceContext->setRenderTarget(m_sceneTarget->getRenderTarget(), m_sceneTarget->getDepthStencil());
    m_deviceContext->setViewport(m_sceneTarget->getSize());
    if (isHardwareMSAA(m_antiAliasing)) {
        m_msaaDirty = true;
    }
    
    CameraData cameraData = {
        cameraComponent->getViewMatrix(),
        cameraComponent->getProjectionMatrix(),
        Vec4{camera.getComponent<TransformComponent>()->getPosition()}
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData, sizeof(CameraData));
    m_deviceContext->setConstantBuffer(*m_cameraBuffer, CAMERA_CONSTANT_BUFFER_SLOT);

    uint32 lightCount = getLights(entities);
    SceneData sceneData = {
        deltaTime,
        lightCount
    };
    m_deviceContext->updateConstantBuffer(*m_sceneBuffer, &sceneData, sizeof(SceneData));
    m_deviceContext->setConstantBuffer(*m_sceneBuffer, SCENE_CONSTANT_BUFFER_SLOT);

    m_deviceContext->setRasterizerState(*m_sceneRasterizer);
    m_deviceContext->setDepthStencilState(m_states->depthDefault());
    m_deviceContext->setSamplerState(*m_sceneSampler, DEFAULT_SAMPLER_STATE_SLOT);

    renderEntities(entities);
}

void GraphicsEngine::render(DebugRenderer& debug, Entity& camera)
{
    CameraComponent* cameraComponent = camera.getComponent<CameraComponent>();
    if (!cameraComponent) {
        return;
    }

    m_deviceContext->setRenderTarget(m_sceneTarget->getRenderTarget(), m_sceneTarget->getDepthStencil());
    m_deviceContext->setViewport(m_sceneTarget->getSize());
    if (isHardwareMSAA(m_antiAliasing)) {
        m_msaaDirty = true;
    }

    CameraData cameraData = {
        cameraComponent->getViewMatrix(),
        cameraComponent->getProjectionMatrix(),
        Vec4{camera.getComponent<TransformComponent>()->getPosition()}
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData, sizeof(CameraData));
    m_deviceContext->setConstantBuffer(*m_cameraBuffer, CAMERA_CONSTANT_BUFFER_SLOT);

    m_deviceContext->setRasterizerState(*m_sceneRasterizer);
    m_deviceContext->setDepthStencilState(m_states->depthDefault());
    m_deviceContext->setSamplerState(*m_sceneSampler, DEFAULT_SAMPLER_STATE_SLOT);

    debug.render(*m_deviceContext);
}

void GraphicsEngine::render(SkyBox& skybox, Entity& camera)
{
    CameraComponent* cameraComponent = camera.getComponent<CameraComponent>();
    if (!cameraComponent) {
        return;
    }

    m_deviceContext->setRenderTarget(m_sceneTarget->getRenderTarget(), m_sceneTarget->getDepthStencil());
    m_deviceContext->setViewport(m_sceneTarget->getSize());
    if (isHardwareMSAA(m_antiAliasing)) {
        m_msaaDirty = true;
    }

    CameraData cameraData = {
        cameraComponent->getViewMatrix(),
        cameraComponent->getProjectionMatrix(),
        Vec4{camera.getComponent<TransformComponent>()->getPosition()}
    };
    m_deviceContext->updateConstantBuffer(*m_cameraBuffer, &cameraData, sizeof(CameraData));
    m_deviceContext->setConstantBuffer(*m_cameraBuffer, CAMERA_CONSTANT_BUFFER_SLOT);

    m_deviceContext->setRasterizerState(m_states->rasterizerSkybox());
    m_deviceContext->setDepthStencilState(m_states->depthSkybox());
    m_deviceContext->setSamplerState(skybox.getSamplerState(), DEFAULT_SAMPLER_STATE_SLOT);
    m_deviceContext->setGraphicsPipelineState(m_states->cubeSkyboxPipeline());
    m_deviceContext->setTexture(skybox.getCubeTexture(), DEFAULT_TEXTURE_SLOT);

    auto& skyboxMesh = m_meshes->skyboxCube();
    m_deviceContext->setVertexBuffer(*skyboxMesh.vertices);
    m_deviceContext->setIndexBuffer(*skyboxMesh.indices);

    m_deviceContext->drawIndexed(skyboxMesh.indices->getIndexCount());
}

void GraphicsEngine::render(UIManager& ui)
{
    m_deviceContext->setRenderTarget(m_sceneTarget->getRenderTarget(), m_sceneTarget->getDepthStencil());
    m_deviceContext->setViewport(m_sceneTarget->getSize());
    if (isHardwareMSAA(m_antiAliasing)) {
        m_msaaDirty = true;
    }

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
    if (m_msaaDirty) {
        m_deviceContext->resolveTexture(m_msaaBuffer->getRenderTarget(), m_primaryBuffer->getRenderTarget());
        m_msaaDirty = false;
    }

    applyPostProcess(effect, *m_primaryBuffer, *m_secondaryBuffer);
    std::swap(m_primaryBuffer, m_secondaryBuffer);
}

void GraphicsEngine::present(Display& display)
{
    if (m_msaaDirty) {
        m_deviceContext->resolveTexture(m_msaaBuffer->getRenderTarget(), m_primaryBuffer->getRenderTarget());
        m_msaaDirty = false;
    }

    const SwapChain& swapChain = display.getSwapChain();
    m_deviceContext->clearAndSetBackBuffer(swapChain, Vec4{1.0f, 1.0f, 1.0f, 1.0f});
    m_deviceContext->setViewport(swapChain.getSize());

    m_deviceContext->setRasterizerState(m_states->rasterizerSolid());
    m_deviceContext->setDepthStencilState(m_states->depthDefault());
    m_deviceContext->setSamplerState(m_states->pointClamp(), DEFAULT_SAMPLER_STATE_SLOT);
    m_deviceContext->setGraphicsPipelineState(m_states->frameBufferPipeline());
    m_deviceContext->setTexture(m_primaryBuffer->getRenderTarget());
    
    m_deviceContext->draw(FULLSCREEN_TRIANGLE_VERTEX_COUNT);
    m_graphicsDevice->executeCommandList(*m_deviceContext);

    display.present();
}

uint32 GraphicsEngine::getLights(EntityManager& entities)
{
    Vector<LightData> lights;
    lights.reserve(DEFAULT_MAX_LIGHTS);

    entities.forEachComponent<LightComponent>([&](LightComponent& lightComponent) {
        if (!lightComponent.isEnabled()) {
            return;
        }
        if (lights.size() >= DEFAULT_MAX_LIGHTS) {
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

        TransformComponent* transform = entity.getComponent<TransformComponent>();
        if (!transform) {
            return;
        }

        const Material* material = meshComponent.getMaterial();
        const Mesh* mesh = meshComponent.getMesh();
        if (!material || !mesh) {
            return;
        }

        ObjectData objectData{
           transform->getWorldMatrix()
        };
        m_deviceContext->updateConstantBuffer(*m_objectBuffer, &objectData, sizeof(ObjectData));
        m_deviceContext->setConstantBuffer(*m_objectBuffer, OBJECT_CONSTANT_BUFFER_SLOT);

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

        m_deviceContext->setVertexBuffer(mesh->getVertexBuffer());
        m_deviceContext->setIndexBuffer(mesh->getIndexBuffer());
        m_deviceContext->drawIndexed(mesh->getIndexBuffer().getIndexCount());
    });
}

void GraphicsEngine::applyPostProcess(PostProcess& effect, FrameBuffer& input, FrameBuffer& output)
{
    m_deviceContext->setRenderTarget(output.getRenderTarget(), output.getDepthStencil());
    m_deviceContext->setViewport(output.getSize());

    m_deviceContext->setRasterizerState(m_states->rasterizerSolid());
    m_deviceContext->setDepthStencilState(m_states->depthDisabled());
    m_deviceContext->setSamplerState(m_states->pointClamp(), DEFAULT_SAMPLER_STATE_SLOT);
    m_deviceContext->setGraphicsPipelineState(effect.getGraphicsPipelineState());

    m_deviceContext->setTexture(input.getRenderTarget());
    if (effect.hasProperties()) {
        if (effect.isDirty()) {
            BinaryData data = effect.getData();
            m_deviceContext->updateConstantBuffer(effect.getProperties(), data.data, static_cast<uint32>(data.size));
            effect.clearDirty();
        }
        m_deviceContext->setConstantBuffer(effect.getProperties(), POST_PROCESSING_EFFECT_CONSTANT_BUFFER_SLOT);
    }

    m_deviceContext->draw(FULLSCREEN_TRIANGLE_VERTEX_COUNT);
}