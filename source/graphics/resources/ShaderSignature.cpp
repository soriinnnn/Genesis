#include <graphics/resources/ShaderSignature.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;
using namespace std;

ShaderSignature::ShaderSignature(const ShaderSignatureDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	BinaryData shaderData = sdesc.shaderBinary.getData();

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		D3DReflect(
			shaderData.data,
			shaderData.size,
			IID_PPV_ARGS(&m_shaderReflection)
		),
		"D3DReflect failed."
	);

	D3D11_SHADER_DESC shaderDesc{};
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_shaderReflection->GetDesc(&shaderDesc),
		"ID3D11ShaderReflection::GetDesc failed."
	);

	m_inputElements.reserve(shaderDesc.InputParameters);
	for (uint32 i = 0; i < shaderDesc.InputParameters; i++) {
		D3D11_SIGNATURE_PARAMETER_DESC param{};
		GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
			m_shaderReflection->GetInputParameterDesc(i, &param),
			"ID3D11ShaderReflection::GetInputParameterDesc failed."
		);

		if (param.SystemValueType != D3D_NAME_UNDEFINED) {
			continue;
		}

		m_inputElements.push_back({
			param.SemanticName,
			param.SemanticIndex,
			graphicsUtils::getDXGIFormatFromMask(param.ComponentType, param.Mask),
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		});
	}

	for (uint32 i = 0; i < shaderDesc.ConstantBuffers; i++) {
		auto* cbuffer = m_shaderReflection->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC cbufferDesc{};
		GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
			cbuffer->GetDesc(&cbufferDesc),
			"ID3D11ShaderReflectionConstantBuffer::GetDesc failed."
		);
		if (!cbufferDesc.Name) {
			continue;
		}

		ShaderReflectionConstantBuffer cbufferReflection{};
		cbufferReflection.size = cbufferDesc.Size;

		D3D11_SHADER_INPUT_BIND_DESC bindDesc{};
		GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
			m_shaderReflection->GetResourceBindingDescByName(cbufferDesc.Name, &bindDesc),
			"ID3D11ShaderReflection::GetResourceBindingDesc failed."
		);
		cbufferReflection.slot = bindDesc.BindPoint;

		for (uint32 j = 0; j < cbufferDesc.Variables; j++) {
			auto* variable = cbuffer->GetVariableByIndex(j);
			auto* type = variable->GetType();

			D3D11_SHADER_VARIABLE_DESC varDesc;
			GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
				variable->GetDesc(&varDesc),
				"ID3D11ShaderReflectionVariable::GetDesc failed."
			);

			/*
			D3D11_SHADER_TYPE_DESC typeDesc;
			GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
				type->GetDesc(&typeDesc),
				"ID3D11ShaderReflectionType::GetDesc failed."
			)
			*/

			cbufferReflection.variables[varDesc.Name] = {varDesc.StartOffset, varDesc.Size};
		}

		m_constantBuffers[cbufferDesc.Name] = cbufferReflection;
	}
}

ShaderSignature::~ShaderSignature() {}

bool ShaderSignature::hasConstantBuffer(const char* name) const
{
	return m_constantBuffers.contains(name);
}

uint32 ShaderSignature::getConstantBufferSlot(const char* name) const
{
	auto it = m_constantBuffers.find(name);
	if (it == m_constantBuffers.end()) {
		GENESIS_LOG_THROW_ERROR("Constant buffer \"{}\" does not exist.", name);
	}
	return it->second.slot;
}

const ShaderReflectionConstantBuffer* ShaderSignature::getConstantBufferReflection(const char* name) const
{
	auto it = m_constantBuffers.find(name);
	if (it == m_constantBuffers.end()) {
		return nullptr;
	}
	return &it->second;
}

BinaryData ShaderSignature::getInputElementsData() const noexcept
{
	return BinaryData{
		reinterpret_cast<const void*>(m_inputElements.data()),
		m_inputElements.size()
	};
}
