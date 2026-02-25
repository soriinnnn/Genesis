#ifndef GENESIS_CORE_H
#define GENESIS_CORE_H
#include <cstdint>
#include <memory>
#include <functional>

namespace genesis 
{
    class Base;
    class Logger;
    class Game;
    class Window;
    class Display;
    class GraphicsEngine;
    class GraphicsDevice;
    class DeviceContext;
    class SwapChain;
    class ShaderBinary;
    class GraphicsPipelineState;
    class VertexBuffer;
    class VertexShaderSignature;
    class ConstantBuffer;

    typedef int8_t  int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    typedef uint8_t  uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    template<typename T> using SharedPtr = std::shared_ptr<T>;
    template<typename T> using UniquePtr = std::shared_ptr<T>;
}

#endif