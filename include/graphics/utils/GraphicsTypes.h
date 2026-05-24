#ifndef GENESIS_GRAPHICS_TYPES_H
#define GENESIS_GRAPHICS_TYPES_H
#include <math/Vec4.h>
#include <math/Vec3.h>
#include <math/Vec2.h>

#define GENESIS_VERTEX_COMPONENT_NONE		(0)
#define GENESIS_VERTEX_COMPONENT_POSITION	(1 << 0)
#define GENESIS_VERTEX_COMPONENT_NORMAL		(1 << 1)
#define GENESIS_VERTEX_COMPONENT_TANGENT	(1 << 2)
#define GENESIS_VERTEX_COMPONENT_BITANGENT	(1 << 3)
#define GENESIS_VERTEX_COMPONENT_TEXCOORD	(1 << 4)
#define GENESIS_VERTEX_COMPONENT_COLOR		(1 << 5)

#define GENESIS_VERTEX_COMPONENT_POSITION_SIZE	sizeof(Vec3)
#define GENESIS_VERTEX_COMPONENT_NORMAL_SIZE	sizeof(Vec3)
#define GENESIS_VERTEX_COMPONENT_TANGENT_SIZE	sizeof(Vec3)
#define GENESIS_VERTEX_COMPONENT_BITANGENT_SIZE	sizeof(Vec3)
#define GENESIS_VERTEX_COMPONENT_TEXCOORD_SIZE	sizeof(Vec2)
#define GENESIS_VERTEX_COMPONENT_COLOR_SIZE		sizeof(Vec4)

#define GENESIS_VERTEX_PRESET_DEFAULT (GENESIS_VERTEX_COMPONENT_POSITION | GENESIS_VERTEX_COMPONENT_NORMAL | GENESIS_VERTEX_COMPONENT_TEXCOORD)
#define GENESIS_VERTEX_PRESET_NORMAL_MAPPED	(GENESIS_VERTEX_PRESET_DEFAULT | GENESIS_VERTEX_COMPONENT_TANGENT | GENESIS_VERTEX_COMPONENT_BITANGENT)

namespace genesis
{
	struct GraphicsContext
	{
		GraphicsDevice& graphicsDevice;
		EngineShaders& engineShaders;
		EngineStates& engineStates;
	};

	enum class ResourceUsage
	{
		Default,
		Immutable,
		Dynamic,
		Staging
	};

	enum class IndexFormat
	{
		UnsignedInt16,
		UnsignedInt32
	};

	enum class PrimitiveTopology
	{
		Points,
		Lines,
		LinesStrip,
		Triangles,
		TrianglesStrip
	};

	enum class ComparisonFunction
	{
		Never,
		Less,
		LessEqual,
		Equal,
		NotEqual,
		Greater,
		GreaterEqual,
		Always
	};

	enum class TextureFilter
	{
		Point,
		Linear,
		Anisotropic
	};

	enum class TextureAddressMode
	{
		Wrap,
		Mirror,
		Clamp
	};

	enum class ShaderType
	{
		VertexShader,
		PixelShader
	};

	struct ShaderReflectionVariable
	{
		uint32 offset;
		uint32 size;
	};

	struct ShaderReflectionConstantBuffer 
	{
		uint32 slot;
		uint32 size;
		HashMap<String, ShaderReflectionVariable> variables;
	};
}

#endif