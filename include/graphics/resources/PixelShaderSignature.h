#ifndef GENESIS_PIXEL_SHADER_SIGNATURE_H
#define GENESIS_PIXEL_SHADER_SIGNATURE_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>
#include <core/utils/Types.h>
#include <d3dcompiler.h>

namespace genesis
{
	struct PixelShaderSignatureDesc
	{
		SharedPtr<ShaderBinary> pixelShader;
	};

	class PixelShaderSignature final: public GraphicsResource
	{
	public:
		PixelShaderSignature(const PixelShaderSignatureDesc& sdesc, const GraphicsResourceDesc& gdesc);
		~PixelShaderSignature() override;

		BinaryData getShaderBinaryData() const noexcept;

		bool hasConstantBuffer(const char* name) const noexcept;
		const ShaderConstantBuffer* getConstantBuffer(const char* name) const;

	private:
		SharedPtr<ShaderBinary> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_shaderReflection;
		std::unordered_map<std::string, ShaderConstantBuffer> m_constantBuffers;
	};
}

#endif