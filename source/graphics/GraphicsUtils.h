#ifndef GENESIS_GRAPHICS_UTILS_H
#define GENESIS_GRAPHICS_UTILS_H
#include <core/Common.h>

namespace genesis
{
	namespace graphicsUtils 
	{
		inline const char* getShaderModelTarget(ShaderType type)
		{
			switch (type)
			{
			case ShaderType::VertexShader:
				return "vs_5_0";
			case ShaderType::PixelShader:
				return "ps_5_0";
			default:
				return "";
			}
		}
	}
}

#endif