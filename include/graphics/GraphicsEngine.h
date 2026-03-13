#ifndef GENESIS_GRAPHICS_ENGINE_H
#define GENESIS_GRAPHICS_ENGINE_H
#include <core/Base.h>
#include <core/Core.h>
#include <math/Vec2.h>
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
        ~GraphicsEngine() override;

        GraphicsDevice& getGraphicsDevice() noexcept;
        void render(Mesh& mesh, Texture& tex, SwapChain& swapChain, float deltaTime);


        // PROVA
        void onKeyDown(Key key) override;
        void onKeyUp(Key key) override;
        void onMouseMove(Point delta, Point pos) override;
        void onMouseDown(MouseButton button, Point pos) override;
        void onMouseUp(MouseButton button, Point pos) override;

        void setGraphicsPipeline(ResourceManager& resourceManager);


    private:
        struct alignas(16) ConstantData
        {
            Mat4 world;
            Mat4 view;
            Mat4 projection;
            Vec3 lightDirection;
            Vec3 camPos;
        };

    private:
        UniquePtr<GraphicsDevice> m_graphicsDevice;
        SharedPtr<DeviceContext> m_deviceContext;
        SharedPtr<GraphicsPipelineState> m_graphicsPipeline;
        SharedPtr<ConstantBuffer> m_constantBuffer;

        Vec3 m_pos{}, m_rot{};
        float m_scale{};
        float m_deltaTime{};
    };
}

#endif