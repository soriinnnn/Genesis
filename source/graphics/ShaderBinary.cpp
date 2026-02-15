#include <graphics/ShaderBinary.h>
#include <graphics/GraphicsUtils.h>
#include <graphics/GraphicsLogUtils.h>
#include <d3dcompiler.h>

using namespace genesis;

// --------------------------------------------------------------------------------

ShaderBinary::ShaderBinary(const ShaderCompileDesc& sDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (!sDesc.shaderSourceName) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source name provided.");
	}
	if (!sDesc.shaderSourceCode) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source code provided.");
	}
	if (!sDesc.shaderSourceCodeSize) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source code size provided.");
	}
	if (!sDesc.shaderEntryPoint) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader entry point provided.");
	}

	UINT compileFlags = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif

	GENESIS_GRAPHICS_CHECK_SHADER_COMPILE(
		D3DCompile(
			sDesc.shaderSourceCode,
			sDesc.shaderSourceCodeSize,
			sDesc.shaderSourceName,
			nullptr,
			nullptr,
			sDesc.shaderEntryPoint,
			graphicsUtils::getShaderModelTarget(sDesc.shaderType),
			compileFlags,
			0,
			&m_blob,
			&errorBlob
		),
		errorBlob.Get()
	);

	m_type = sDesc.shaderType;
}

ShaderBinary::~ShaderBinary() {}

ShaderBinaryData ShaderBinary::getData() const noexcept
{
	return ShaderBinaryData{m_blob->GetBufferPointer(), m_blob->GetBufferSize()};
}

ShaderType ShaderBinary::getType() const noexcept
{
	return m_type;
}
