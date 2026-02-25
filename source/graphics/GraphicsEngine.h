#ifndef GENESIS_GRAPHICS_ENGINE_H
#define GENESIS_GRAPHICS_ENGINE_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Vec3.h>
#include <math/Vec4.h>

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
        virtual ~GraphicsEngine() override;

        GraphicsDevice& getGraphicsDevice() noexcept;

        void clearPipeline();
        void render(SwapChain& swapChain);

    private:
        struct Vertex
        {
            Vec3 position;
            Vec4 color;
        };

        struct alignas(16) ConstantData
        {
            float scale{};
        };

    private:
        SharedPtr<GraphicsDevice> m_graphicsDevice;
        SharedPtr<DeviceContext> m_deviceContext;
        SharedPtr<GraphicsPipelineState> m_graphicsPipeline;
        SharedPtr<VertexBuffer> m_vertexBuffer;
        SharedPtr<ConstantBuffer> m_constantBuffer;
    };
}

#endif