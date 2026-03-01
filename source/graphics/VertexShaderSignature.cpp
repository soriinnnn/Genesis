#include <graphics/VertexShaderSignature.h>
#include <graphics/ShaderBinary.h>
#include <graphics/GraphicsUtils.h>
#include <graphics/GraphicsLogUtils.h>
#include <d3dcompiler.h>

using namespace genesis;

VertexShaderSignature::VertexShaderSignature(const VertexShaderSignatureDesc& sDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (!sDesc.vsBinary) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader binary provided.");
	}
	if (sDesc.vsBinary->getType() != ShaderType::VertexShader) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex shader type is invalid.");
	}
	m_vertexShader = sDesc.vsBinary;

	BinaryData vertexShaderData = m_vertexShader->getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		D3DReflect(
			vertexShaderData.m_data,
			vertexShaderData.dataSize,
			IID_PPV_ARGS(&m_shaderReflection)
		),
		"D3DReflect failed."
	);

	D3D11_SHADER_DESC shaderDesc{};
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_shaderReflection->GetDesc(&shaderDesc),
		"ID3D11ShaderReflection::GetDesc failed."
	);
	
	D3D11_SIGNATURE_PARAMETER_DESC params[D3D11_STANDARD_VERTEX_ELEMENT_COUNT]{};
	m_numElements = shaderDesc.InputParameters;
	for (uint32 i = 0; i < m_numElements; i++) {
		GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
			m_shaderReflection->GetInputParameterDesc(i, &params[i]),
			"ID3D11ShaderReflection::GetInputParameterDesc failed."
		);
	}

	for (uint32 i = 0; i < m_numElements; i++) {
		auto param = params[i];
		m_elements[i] = {
			param.SemanticName,
			param.SemanticIndex,
			graphicsUtils::getDXGIFormatFromMask(param.ComponentType, param.Mask),
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		};
	}
}

VertexShaderSignature::~VertexShaderSignature() {}

BinaryData VertexShaderSignature::getShaderBinaryData() const noexcept
{
	return m_vertexShader->getData();
}

BinaryData VertexShaderSignature::getInputElementsData() const noexcept
{
	return BinaryData{
		m_elements,
		m_numElements
	};
}
