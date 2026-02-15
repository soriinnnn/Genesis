#ifndef GENESIS_CORE_H
#define GENESIS_CORE_H
#include <stdexcept>
#include <memory>

namespace genesis 
{
    class Base;

    class Game;
    class Display;
    class Window;

    class GraphicsEngine;
    class GraphicsDevice;
    class DeviceContext;
    class SwapChain;
    class ShaderBinary;
    class GraphicsPipelineState;
    class VertexBuffer;

    class Logger;

    using GraphicsDevicePtr = std::shared_ptr<GraphicsDevice>;
    using DeviceContextPtr = std::shared_ptr<DeviceContext>;
    using SwapChainPtr = std::shared_ptr<SwapChain>;
    using ShaderBinaryPtr = std::shared_ptr<ShaderBinary>;
    using GraphicsPipelineStatePtr = std::shared_ptr<GraphicsPipelineState>;
    using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
}

#endif