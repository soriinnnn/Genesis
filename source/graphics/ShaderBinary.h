#ifndef GENESIS_SHADER_BINARY_H
#define GENESIS_SHADER_BINARY_H
#include <graphics/GraphicsResource.h>

namespace genesis
{
	class ShaderBinary final: public GraphicsResource
	{
	public:
		ShaderBinary(const ShaderCompileDesc& sDesc, const GraphicsResourceDesc& grDesc);
		~ShaderBinary() override;

		ShaderBinaryData getData() const noexcept;
		ShaderType getType() const noexcept;

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
		ShaderType m_type;
	};
}

#endif