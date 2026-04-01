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

        void render(World& world);
        void render(World& world, SwapChain& swapChain);
        void postProcess(PostProcess& effect);
        void present(SwapChain& swapChain);

    private:
        void renderEntities(World& world);
        void applyPostProcess(PostProcess& effect, FrameBuffer& input, FrameBuffer& output);

    private:
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
            Vec4 direction;
            Vec4 color;
            float intensity;
        };

    private:
        UniquePtr<GraphicsDevice> m_graphicsDevice;
        UniquePtr<EngineShaders> m_engineShaders;
        UniquePtr<FrameBuffer> m_primaryBuffer;
        UniquePtr<FrameBuffer> m_secondaryBuffer;

    private:
        SharedPtr<DeviceContext> m_deviceContext;
        SharedPtr<ConstantBuffer> m_cameraBuffer;
        SharedPtr<ConstantBuffer> m_objectBuffer;
        SharedPtr<ConstantBuffer> m_lightBuffer;
        SharedPtr<SamplerState> m_pointSampler;
        SharedPtr<GraphicsPipelineState> m_framePipeline;
    };
}

#endif