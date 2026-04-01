#ifndef GENESIS_SHADER_BINARY_H
#define GENESIS_SHADER_BINARY_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>
#include <core/utils/Types.h>

namespace genesis
{
	struct ShaderBinaryDesc
	{
		const void* binaryCode{};
		size_t binaryCodeSize{};
	};

	struct ShaderCompileDesc
	{
		const char* sourceName{};
		const void* sourceCode{};
		size_t sourceCodeSize{};
		const char* entryPoint{};
		ShaderType type{};
	};

	class ShaderBinary final: public GraphicsResource
	{
	public:
		ShaderBinary(const ShaderBinaryDesc& sdesc, const GraphicsResourceDesc& gdesc);
		ShaderBinary(const ShaderCompileDesc& sdesc, const GraphicsResourceDesc& gdesc);
		~ShaderBinary() override;

		BinaryData getData() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_binary;
	};
}

#endif