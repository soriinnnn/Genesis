#ifndef GENESIS_GRAPHICS_UTILS_H
#define GENESIS_GRAPHICS_UTILS_H
#include <core/Common.h>
#include <d3d11.h>
#include <bit>

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

		inline DXGI_FORMAT getDXGIFormatFromMask(D3D_REGISTER_COMPONENT_TYPE type, UINT mask)
		{
			int componentCount = std::popcount(mask);
			if (componentCount < 1) {
				return DXGI_FORMAT_UNKNOWN;
			}

			constexpr DXGI_FORMAT formatTable[1][4] = {
				{
					DXGI_FORMAT_R32_FLOAT,
					DXGI_FORMAT_R32G32_FLOAT,
					DXGI_FORMAT_R32G32B32_FLOAT,
					DXGI_FORMAT_R32G32B32A32_FLOAT
				}
			};

			int typeIndex = 0;
			switch (type)
			{
			case D3D_REGISTER_COMPONENT_FLOAT32: 
				typeIndex = 0;
				break;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}

			return formatTable[typeIndex][componentCount - 1];
		}
	}
}

#endif