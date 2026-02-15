#ifndef GENESIS_GRAPHICS_ENGINE_H
#define GENESIS_GRAPHICS_ENGINE_H
#include <core/Core.h>
#include <core/Common.h>
#include <core/Base.h>

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
        GraphicsDevicePtr m_graphicsDevice;
        DeviceContextPtr m_deviceContext;
        GraphicsPipelineStatePtr m_graphicsPipeline;
        VertexBufferPtr m_vertexBuffer;
    };
}

#endif