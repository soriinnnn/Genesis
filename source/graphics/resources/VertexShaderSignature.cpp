#include <graphics/resources/VertexShaderSignature.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <graphics/utils/GraphicsUtils.h>

using namespace genesis;

VertexShaderSignature::VertexShaderSignature(const VertexShaderSignatureDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!sdesc.vertexShader) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader binary provided.");
	}
	if (sdesc.vertexShader->getType() != ShaderType::VertexShader) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid vertex shader type.");
	}
	m_vertexShader = sdesc.vertexShader;

	BinaryData vertexShaderData = m_vertexShader->getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		D3DReflect(
			vertexShaderData.data,
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

	for (uint32 i = 0; i < shaderDesc.ConstantBuffers; i++) {
		auto constantBuffer = m_shaderReflection->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC constDesc{};
		GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
			constantBuffer->GetDesc(&constDesc),
			"ID3D11ShaderReflectionConstantBuffer::GetDesc failed."
		);
		if (!constDesc.Name) {
			continue;
		}

		ShaderReflectionConstantBuffer cbuffer{};
		cbuffer.size = constDesc.Size;

		D3D11_SHADER_INPUT_BIND_DESC bindDesc{};
		GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
			m_shaderReflection->GetResourceBindingDescByName(constDesc.Name, &bindDesc),
			"ID3D11ShaderReflection::GetResourceBindingDesc failed."
		);
		cbuffer.slot = bindDesc.BindPoint;

		for (uint32 j = 0; j < constDesc.Variables; j++) {
			auto variable = constantBuffer->GetVariableByIndex(j);

			D3D11_SHADER_VARIABLE_DESC varDesc;
			GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
				variable->GetDesc(&varDesc),
				"ID3D11ShaderReflectionVariable::GetDesc failed."
			);
			cbuffer.variables[varDesc.Name] = {varDesc.StartOffset, varDesc.Size};
		}
		
		m_constantBuffers[constDesc.Name] = cbuffer;
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

bool VertexShaderSignature::hasConstantBuffer(const char* name) const noexcept
{
	return m_constantBuffers.contains(name);
}

const ShaderReflectionConstantBuffer* VertexShaderSignature::getConstantBufferReflection(const char* name) const
{
	auto it = m_constantBuffers.find(name);
	if (it == m_constantBuffers.end()) {
		return nullptr;
	}

	return &it->second;
}