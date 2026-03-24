#ifndef GENESIS_GRAPHICS_TYPES_H
#define GENESIS_GRAPHICS_TYPES_H
#include <math/Vec3.h>
#include <math/Vec2.h>

namespace genesis
{
	struct Vertex
	{
		Vec3 position;
		Vec3 normal;
		Vec2 texture;
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

	enum class DepthComparison
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

	struct ShaderReflectionConstantBuffer {
		uint32 slot;
		uint32 size;
		std::unordered_map<std::string, ShaderReflectionVariable> variables;
	};
}

#endif