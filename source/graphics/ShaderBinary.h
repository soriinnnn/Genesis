#ifndef GENESIS_SHADER_BINARY_H
#define GENESIS_SHADER_BINARY_H
#include <core/Common.h>
#include <graphics/GraphicsResource.h>
#include <graphics/GraphicsTypes.h>

namespace genesis
{
	struct ShaderCompileDesc
	{
		const char* shaderSourceName{};
		const void* shaderSourceCode{};
		size_t shaderSourceCodeSize{};
		const char* shaderEntryPoint{};
		ShaderType shaderType{};
	};

	class ShaderBinary final: public GraphicsResource
	{
	public:
		ShaderBinary(const ShaderCompileDesc& sDesc, const GraphicsResourceDesc& grDesc);
		~ShaderBinary() override;

		BinaryData getData() const noexcept;
		ShaderType getType() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
		ShaderType m_type;
	};
}

#endif