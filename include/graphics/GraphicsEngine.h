#ifndef GENESIS_GRAPHICS_ENGINE_H
#define GENESIS_GRAPHICS_ENGINE_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Rect.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Vec4.h>
#include <math/Mat4.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
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
        void resizeFrameBuffers(uint32 width, uint32 height);

        void clear(Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f});
        void render(World& world, float deltaTime);
        void render(UIManager& ui);
        void postProcess(PostProcess& effect);
        void present(SwapChain& swapChain);

    private:
        void renderEntities(World& world);
        void applyPostProcess(PostProcess& effect, FrameBuffer& input, FrameBuffer& output);

    private:
        struct alignas(16) SceneData
        {
            float deltaTime;
            uint32 lightCount;
        };

        struct alignas(16) CameraData
        {
            Mat4 view;
            Mat4 projection;
            Vec3 camPos;
        };

        struct alignas(16) ObjectData
        {
            Mat4 world;
        };

        struct alignas(16) LightData
        {
            Vec3 position;      int pd0;
            Vec3 direction;     int pd1;
            Vec3 color;
            float radius;
            float intensity;
            int type;
        };

    private:
        SharedPtr<GraphicsDevice> m_graphicsDevice;
        UniquePtr<EngineShaders> m_engineShaders;
        UniquePtr<FrameBuffer> m_primaryBuffer;
        UniquePtr<FrameBuffer> m_secondaryBuffer;

    private:
        SharedPtr<DeviceContext> m_deviceContext;
        SharedPtr<SpriteBatch> m_spriteBatch;
        SharedPtr<ConstantBuffer> m_sceneBuffer;
        SharedPtr<ConstantBuffer> m_cameraBuffer;
        SharedPtr<ConstantBuffer> m_objectBuffer;
        SharedPtr<StructuredBuffer> m_lightsBuffer;
        SharedPtr<SamplerState> m_pointClampSampler;
        SharedPtr<GraphicsPipelineState> m_framePipeline;
    };
}

#endif