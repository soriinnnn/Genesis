#ifndef GENESIS_SHADER_BINARY_H
#define GENESIS_SHADER_BINARY_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>
#include <core/utils/Types.h>

namespace genesis
{
	struct ShaderBinaryDesc
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
		ShaderBinary(const ShaderBinaryDesc& sdesc, const GraphicsResourceDesc& gdesc);
		~ShaderBinary() override;

		BinaryData getData() const noexcept;
		ShaderType getType() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
		ShaderType m_type;
	};
}

#endif