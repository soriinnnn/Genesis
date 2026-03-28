#ifndef GENESIS_GRAPHICS_ENGINE_H
#define GENESIS_GRAPHICS_ENGINE_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Vec4.h>
#include <math/Mat4.h>

namespace genesis
{
    struct GraphicsEngineDesc
    {
        BaseDesc base;
    };

    class GraphicsEngine final: public Base
    {
    public:
        explicit GraphicsEngine(const GraphicsEngineDesc& desc);
        ~GraphicsEngine() override;

        GraphicsDevice& getGraphicsDevice() noexcept;
        void render(World& world, SwapChain& swapChain, float deltaTime);

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
        SharedPtr<DeviceContext> m_deviceContext;
        SharedPtr<ConstantBuffer> m_cameraBuffer;
        SharedPtr<ConstantBuffer> m_objectBuffer;
        SharedPtr<ConstantBuffer> m_lightBuffer;
    };
}

#endif