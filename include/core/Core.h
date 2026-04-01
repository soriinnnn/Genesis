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

    class Display;
    class Window;

    class InputManager;
    class InputListener;

    class ResourceManager;
    class Resource;
    class Mesh;
    class Shader;
    class Texture;
    class Material;
    class PostProcess;

    class EntityManager;
    class Entity;
    class Component;
    class Transform;
    class MeshRenderer;

    class Player;
    class PlayerController;

    class GraphicsEngine;
    class GraphicsDevice;
    class EngineShaders;
    class FrameBuffer;
    class GraphicsCache;
    class PipelineStateCache;
    class SamplerStateCache;

    class GraphicsResource;
    class DeviceContext;
    class SwapChain;
    class ShaderBinary;
    class ShaderSignature;
    class VertexBuffer;
    class IndexBuffer;
    class ConstantBuffer;
    class GraphicsPipelineState;
    class SamplerState;
    class ImageTexture;
    class RenderTargetTexture;
    class DepthStencilTexture;

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
    template<typename T> using WeakPtr = std::weak_ptr<T>;

    using String = std::string;
    template<typename T> using Vector = std::vector<T>;
    template<typename K, typename T> using HashMap = std::unordered_map<K, T>;
}

#endif