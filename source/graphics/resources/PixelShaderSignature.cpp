#include <graphics/resources/PixelShaderSignature.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <graphics/utils/GraphicsUtils.h>

using namespace genesis;

PixelShaderSignature::PixelShaderSignature(const PixelShaderSignatureDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!sdesc.pixelShader) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader binary provided.");
	}
	if (sdesc.pixelShader->getType() != ShaderType::PixelShader) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid vertex shader type.");
	}
	m_pixelShader = sdesc.pixelShader;

	BinaryData pixelShaderData = m_pixelShader->getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		D3DReflect(
			pixelShaderData.m_data,
			pixelShaderData.dataSize,
			IID_PPV_ARGS(&m_shaderReflection)
		),
		"D3DReflect failed."
	);

	D3D11_SHADER_DESC shaderDesc{};
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_shaderReflection->GetDesc(&shaderDesc),
		"ID3D11ShaderReflection::GetDesc failed."
	);

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

		ShaderConstantBuffer cbuffer{};
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

PixelShaderSignature::~PixelShaderSignature() {}

BinaryData PixelShaderSignature::getShaderBinaryData() const noexcept
{
    return m_pixelShader->getData();
}

bool PixelShaderSignature::hasConstantBuffer(const char* name) const noexcept
{
	return m_constantBuffers.contains(name);
}

const ShaderConstantBuffer* PixelShaderSignature::getConstantBuffer(const char* name) const
{
	auto it = m_constantBuffers.find(name);
	if (it == m_constantBuffers.end()) {
		return nullptr;
	}

	return &it->second;
}
