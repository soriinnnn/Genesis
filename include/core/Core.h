#ifndef GENESIS_CORE_H
#define GENESIS_CORE_H
#include <stdexcept>
#include <cstdint>
#include <memory>

namespace genesis 
{
    class Base;
    class Logger;
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
    class VertexShaderSignature;

    using WindowPtr = std::shared_ptr<Window>;
    using GraphicsDevicePtr = std::shared_ptr<GraphicsDevice>;
    using DeviceContextPtr = std::shared_ptr<DeviceContext>;
    using SwapChainPtr = std::shared_ptr<SwapChain>;
    using ShaderBinaryPtr = std::shared_ptr<ShaderBinary>;
    using GraphicsPipelineStatePtr = std::shared_ptr<GraphicsPipelineState>;
    using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
    using VertexShaderSignaturePtr = std::shared_ptr<VertexShaderSignature>;

    typedef int8_t  int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    typedef uint8_t  uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;
}

#endif