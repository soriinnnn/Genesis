#ifndef GENESIS_SHADER_SIGNATURE_H
#define GENESIS_SHADER_SIGNATURE_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>
#include <core/utils/Types.h>
#include <d3dcompiler.h>

GENESIS_NAMESPACE_START

struct ShaderSignatureDesc
{
	const ShaderBinary& shaderBinary;
};

class ShaderSignature final: public GraphicsResource
{
public:
	ShaderSignature(const ShaderSignatureDesc& sdesc, const GraphicsResourceDesc& gdesc);
	~ShaderSignature() override;

	bool hasConstantBuffer(const char* name) const;
	uint32 getConstantBufferSlot(const char* name) const;
	const ShaderReflectionConstantBuffer* getConstantBufferReflection(const char* name) const;

	BinaryData getInputElementsData() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> m_shaderReflection;
	Vector<D3D11_INPUT_ELEMENT_DESC> m_inputElements;
	HashMap<String, ShaderReflectionConstantBuffer> m_constantBuffers;
};

GENESIS_NAMESPACE_END

#endif