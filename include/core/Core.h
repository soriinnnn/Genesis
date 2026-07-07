#ifndef GENESIS_CORE_H
#define GENESIS_CORE_H
#include <core/utils/Macros.h>
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
#include <map>

GENESIS_NAMESPACE_BEGIN

class Base;
class Logger;
class Game;

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
class Font;
class SkyBox;

class EntityManager;
class Entity;
class Component;
class TransformComponent;
class MeshRendererComponent;
class LightComponent;
class CameraComponent;
class RigidBodyComponent;
class ScriptComponent;

class ScriptManager;
class Script;

class UIManager;
class UIElement;
class UILabel;
class UIImage;
class UIPanel;
class UIButton;

class PhysicsEngine;
class RigidBody;

class GraphicsEngine;
class GraphicsDevice;
class FrameBuffer;
class EngineShaders;
class EngineStates;
class EngineMeshes;

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
class StructuredBuffer;
class RasterizerState;
class DepthStencilState;
class GraphicsPipelineState;
class SamplerState;
class ImageTexture;
class CubeTexture;
class RenderTargetTexture;
class DepthStencilTexture;
class SpriteBatch;
class FontAtlas;

using ResourceId = size_t;
using EntityId = size_t;
using TypeId = size_t;

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

template<typename T> using Vector = std::vector<T>;
template<typename K, typename T> using HashMap = std::unordered_map<K, T>;
template<typename K, typename T> using Map = std::map<K, T>;
template<typename T1, typename T2> using Pair = std::pair<T1, T2>;

using String = std::string;
using WString = std::wstring;
using TimePoint = std::chrono::steady_clock::time_point;

GENESIS_NAMESPACE_END

#endif