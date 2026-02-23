#ifndef GENESIS_GRAPHICS_COMMON_H
#define GENESIS_GRAPHICS_COMMON_H

namespace genesis
{
	enum class ShaderType
	{
		VertexShader,
		PixelShader
	};

	enum class PrimitiveTopology
	{
		Points,
		Lines,
		LinesStrip,
		Triangles,
		TrianglesStrip
	};
}

#endif