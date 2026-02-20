#ifndef GENESIS_GRAPHICS_ENGINE_H
#define GENESIS_GRAPHICS_ENGINE_H
#include <core/Core.h>
#include <core/Common.h>
#include <core/Base.h>
#include <math/Vec3.h>
#include <math/Vec4.h>

namespace genesis
{
    class GraphicsEngine final: public Base 
    {
    public:
        explicit GraphicsEngine(const GraphicsEngineDesc& desc);
        virtual ~GraphicsEngine() override;

        GraphicsDevice& getGraphicsDevice() noexcept;

        void render(SwapChain& swapChain);

    private:
        struct Vertex
        {
            Vec3 position;
            Vec4 color;
        };

    private:
        GraphicsDevicePtr m_graphicsDevice;
        DeviceContextPtr m_deviceContext;
        GraphicsPipelineStatePtr m_graphicsPipeline;
        VertexBufferPtr m_vertexBuffer;
    };
}

#endif