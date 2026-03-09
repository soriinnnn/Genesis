#ifndef GENESIS_VERTEX_SHADER_SIGNATURE_H
#define GENESIS_VERTEX_SHADER_SIGNATURE_H
#include <graphics/resources/GraphicsResource.h>
#include <core/utils/Types.h>
#include <d3dcompiler.h>

namespace genesis
{
	struct VertexShaderSignatureDesc
	{
		const SharedPtr<ShaderBinary>& vsBinary;
	};

	class VertexShaderSignature final: public GraphicsResource
	{
	public:
		VertexShaderSignature(const VertexShaderSignatureDesc& sDesc, const GraphicsResourceDesc& grDesc);
		~VertexShaderSignature() override;

		BinaryData getShaderBinaryData() const noexcept;
		BinaryData getInputElementsData() const noexcept;

	private:
		SharedPtr<ShaderBinary> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_shaderReflection;
		D3D11_INPUT_ELEMENT_DESC m_elements[D3D11_STANDARD_VERTEX_ELEMENT_COUNT];
		uint32 m_numElements;
	};
}

#endif