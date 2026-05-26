#ifndef GENESIS_GRAPHICS_UTILS_H
#define GENESIS_GRAPHICS_UTILS_H
#include <graphics/utils/GraphicsTypes.h>
#include <d3d11.h>
#include <bit>

namespace genesis
{
	namespace graphicsUtils 
	{
		inline const char* getShaderModelTarget(ShaderType type)
		{
			switch (type) {
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

			return formatTable[typeIndex][componentCount-1];
		}

		inline D3D11_USAGE getD3D11Usage(ResourceUsage usage)
		{
			switch (usage) {
			case ResourceUsage::Default:
				return D3D11_USAGE_DEFAULT;
			case ResourceUsage::Immutable:
				return D3D11_USAGE_IMMUTABLE;
			case ResourceUsage::Dynamic:
				return D3D11_USAGE_DYNAMIC;
			case ResourceUsage::Staging:
				return D3D11_USAGE_STAGING;
			default:
				return D3D11_USAGE_DEFAULT;
			}
		}

		inline D3D_PRIMITIVE_TOPOLOGY getD3DPrimitiveTopology(PrimitiveTopology primitive)
		{
			switch (primitive) {
			case PrimitiveTopology::Points:
				return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			case PrimitiveTopology::Lines:
				return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			case PrimitiveTopology::LinesStrip:
				return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			case PrimitiveTopology::Triangles:
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case PrimitiveTopology::TrianglesStrip:
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			default:
				return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			}
		}

		inline D3D11_FILTER getD3D11Filter(TextureFilter filter)
		{
			switch (filter) {
			case TextureFilter::Point:
				return D3D11_FILTER_MIN_MAG_MIP_POINT;
			case TextureFilter::Bilinear:
				return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			case TextureFilter::Trilinear:
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			case TextureFilter::Anisotropic:
				return D3D11_FILTER_ANISOTROPIC;
			default:
				return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			}
		}

		inline D3D11_TEXTURE_ADDRESS_MODE getD3D11TextureAddressMode(TextureAddressMode mode)
		{
			switch (mode) {
			case TextureAddressMode::Wrap:
				return D3D11_TEXTURE_ADDRESS_WRAP;
			case TextureAddressMode::Mirror:
				return D3D11_TEXTURE_ADDRESS_MIRROR;
			case TextureAddressMode::Clamp:
				return D3D11_TEXTURE_ADDRESS_CLAMP;
			default:
				return D3D11_TEXTURE_ADDRESS_WRAP;
			}
		}

		inline D3D11_COMPARISON_FUNC getD3D11ComparisonFunc(ComparisonFunction comparison)
		{
			switch (comparison) {
			case ComparisonFunction::Never:
				return D3D11_COMPARISON_NEVER;
			case ComparisonFunction::Less:
				return D3D11_COMPARISON_LESS;
			case ComparisonFunction::LessEqual:
				return D3D11_COMPARISON_LESS_EQUAL;
			case ComparisonFunction::Equal:
				return D3D11_COMPARISON_EQUAL;
			case ComparisonFunction::NotEqual:
				return D3D11_COMPARISON_NOT_EQUAL;
			case ComparisonFunction::Greater:
				return D3D11_COMPARISON_GREATER;
			case ComparisonFunction::GreaterEqual:
				return D3D11_COMPARISON_GREATER_EQUAL;
			case ComparisonFunction::Always:
				return D3D11_COMPARISON_ALWAYS;
			default:
				return D3D11_COMPARISON_LESS;
			}
		}

		inline DXGI_FORMAT getDXGIIndexFormat(IndexFormat format)
		{
			switch (format) {
			case IndexFormat::UnsignedInt16:
				return DXGI_FORMAT_R16_UINT;
			case IndexFormat::UnsignedInt32:
				return DXGI_FORMAT_R32_UINT;
			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		inline uint32 getIndexSize(IndexFormat format)
		{
			switch (format) {
			case IndexFormat::UnsignedInt16:
				return sizeof(uint16);
			case IndexFormat::UnsignedInt32:
				return sizeof(uint32);
			default:
				return 0;
			}
		}

		inline bool isHardwareMSAA(AntiAliasing antiAliasing) 
		{
			return 
				antiAliasing == AntiAliasing::MSAA_2X || 
				antiAliasing == AntiAliasing::MSAA_4X || 
				antiAliasing == AntiAliasing::MSAA_8X;
		}

		inline uint32 getMSAASampleCount(AntiAliasing antiAliasing)
		{
			switch (antiAliasing) {
			case AntiAliasing::MSAA_2X:
				return 2;
			case AntiAliasing::MSAA_4X:
				return 4;
			case AntiAliasing::MSAA_8X:
				return 8;
			default:
				return 1;
			}
		}
	}
}

#endif