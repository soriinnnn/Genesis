#ifndef GENESIS_GRAPHICS_ENGINE_H
#define GENESIS_GRAPHICS_ENGINE_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Vec3.h>
#include <math/Vec4.h>
#include <math/Mat4.h>

#include <input/InputListener.h>

namespace genesis
{
    struct GraphicsEngineDesc
    {
        BaseDesc base;
    };

    class GraphicsEngine final: public Base, public InputListener
    {
    public:
        explicit GraphicsEngine(const GraphicsEngineDesc& desc);
        virtual ~GraphicsEngine() override;

        GraphicsDevice& getGraphicsDevice() noexcept;

        void clearPipelineState();
        void render(SwapChain& swapChain, float deltaTime);


        // PROVA
        void onKeyDown(Key key) override;
        void onKeyUp(Key key) override;


    private:
        struct Vertex
        {
            Vec3 position;
            Vec4 color;
        };

        struct alignas(16) ConstantData
        {
            Mat4 world;
            Mat4 view;
            Mat4 projection;
        };

    private:
        SharedPtr<GraphicsDevice> m_graphicsDevice;
        SharedPtr<DeviceContext> m_deviceContext;
        SharedPtr<GraphicsPipelineState> m_graphicsPipeline;
        SharedPtr<VertexBuffer> m_vertexBuffer;
        SharedPtr<IndexBuffer> m_indexBuffer;
        SharedPtr<ConstantBuffer> m_constantBuffer;

        Vec3 m_pos{}, m_rot{};
        float m_scale{};
        float m_deltaTime{};
    };
}

#endif