#ifndef GENESIS_CORE_H
#define GENESIS_CORE_H
#include <queue>
#include <chrono>
#include <format>
#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <functional>
#include <unordered_map>

namespace genesis 
{
    class Base;
    class Logger;

    class Game;
    class World;

    class Window;
    class Display;

    class InputManager;
    class InputListener;

    class ResourceManager;
    class Resource;
    class Mesh;
    class Texture;
    class Material;
    class Shader;

    class EntityManager;
    class Entity;
    class Player;
    class Component;
    class Transform;
    class MeshRenderer;

    class GraphicsEngine;
    class GraphicsDevice;

    class GraphicsResource;
    class DeviceContext;
    class SwapChain;
    class ShaderBinary;
    class ShaderSignature;
    class VertexBuffer;
    class IndexBuffer;
    class ConstantBuffer;
    class GraphicsPipelineState;
    class GraphicsTexture;
    class DepthBuffer;

    using ResourceId = size_t;
    using EntityId = size_t;

    typedef int8_t  int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;

    typedef uint8_t  uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    template<typename T> using SharedPtr = std::shared_ptr<T>;
    template<typename T> using UniquePtr = std::unique_ptr<T>;
}

#endif