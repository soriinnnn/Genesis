#ifndef GENESIS_GRAPHICS_TYPES_H
#define GENESIS_GRAPHICS_TYPES_H
#include <math/Vec4.h>
#include <math/Vec3.h>
#include <math/Vec2.h>

namespace genesis
{
	enum class ResourceUsage
	{
		Default,
		Immutable,
		Dynamic,
		Staging
	};

	enum class ShaderType
	{
		VertexShader,
		PixelShader
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
		Bilinear,
		Trilinear,
		Anisotropic
	};

	enum class TextureAddressMode
	{
		Wrap,
		Clamp,
		Mirror
	};

	enum class AntiAliasing
	{
		None,
		MSAA_2X,
		MSAA_4X,
		MSAA_8X
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