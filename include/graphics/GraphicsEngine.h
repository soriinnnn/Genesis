#ifndef GENESIS_GRAPHICS_ENGINE_H
#define GENESIS_GRAPHICS_ENGINE_H
#include <core/Base.h>
#include <core/Core.h>
#include <core/utils/Types.h>
#include <math/Rect.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Vec4.h>
#include <math/Mat4.h>
#include <physics/DebugRenderer.h>

GENESIS_NAMESPACE_BEGIN

struct GraphicsEngineDesc
{
    BaseDesc base;
    Rect buffersSize;
};

class GraphicsEngine final: public Base
{
public:
    explicit GraphicsEngine(const GraphicsEngineDesc& desc);
    ~GraphicsEngine() override;

    GraphicsContext getGraphicsContext() noexcept;

    Rect getRenderResolution() const noexcept;
    AntiAliasing getAntiAliasing() const noexcept;
    TextureFiltering getTextureFiltering() const noexcept;

    void setRenderResolution(const Rect& resolution);
    void setAntiAliasing(AntiAliasing mode);
    void setTextureFiltering(TextureFiltering filter);

    void clear(const Vec4& color = {1.0f, 1.0f, 1.0f, 1.0f});
    void render(EntityManager& entities, Entity& camera, float deltaTime);
    void render(DebugRenderer& debug, Entity& camera);
    void render(UIManager& ui);
    void postProcess(PostProcess& effect);
    void present(Display& display);

private:
    uint32 getLights(EntityManager& entities);
    void renderEntities(EntityManager& entities);
    void applyPostProcess(PostProcess& effect, FrameBuffer& input, FrameBuffer& output);

private:
    struct alignas(16) SceneData
    {
        float deltaTime;
        uint32 lightCount;
    };

    struct alignas(16) CameraData
    {
        Mat4 viewMatrix;
        Mat4 projectionMatrix;
        Vec4 cameraPosition;
    };

    struct alignas(16) ObjectData
    {
        Mat4 worldMatrix;
    };

    struct LightData
    {
        Vec3 position;
        Vec3 direction;
        Vec3 color;
        float radius;
        float intensity;
        int type;
    };

private:
    SharedPtr<GraphicsDevice> m_graphicsDevice;
    UniquePtr<FrameBuffer> m_primaryBuffer;
    UniquePtr<FrameBuffer> m_secondaryBuffer;
    UniquePtr<FrameBuffer> m_msaaBuffer;
    UniquePtr<EngineShaders> m_shaders;
    UniquePtr<EngineStates> m_states;

private:
    SharedPtr<DeviceContext> m_deviceContext;
    SharedPtr<SpriteBatch> m_spriteBatch;
    SharedPtr<ConstantBuffer> m_sceneBuffer;
    SharedPtr<ConstantBuffer> m_cameraBuffer;
    SharedPtr<ConstantBuffer> m_objectBuffer;
    SharedPtr<StructuredBuffer> m_lightsBuffer;

private:
    const RasterizerState* m_sceneRasterizer;
    const SamplerState* m_sceneSampler;
    FrameBuffer* m_sceneTarget;
    AntiAliasing m_antiAliasing;
    TextureFiltering m_textureFiltering;
};

GENESIS_NAMESPACE_END

#endif