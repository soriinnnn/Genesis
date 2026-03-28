#include <graphics/resources/ShaderBinary.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <graphics/utils/GraphicsUtils.h>
#include <d3dcompiler.h>

using namespace genesis;

ShaderBinary::ShaderBinary(const ShaderBinaryDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!sdesc.shaderSourceName) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source name provided.");
	}
	if (!sdesc.shaderSourceCode) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source code provided.");
	}
	if (!sdesc.shaderSourceCodeSize) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source code size provided.");
	}
	if (!sdesc.shaderEntryPoint) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader entry point provided.");
	}

	UINT compileFlags = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif

	GENESIS_GRAPHICS_CHECK_SHADER_COMPILE(
		D3DCompile(
			sdesc.shaderSourceCode,
			sdesc.shaderSourceCodeSize,
			sdesc.shaderSourceName,
			nullptr,
			nullptr,
			sdesc.shaderEntryPoint,
			graphicsUtils::getShaderModelTarget(sdesc.shaderType),
			compileFlags,
			0,
			&m_blob,
			&errorBlob
		),
		errorBlob.Get()
	);

	m_type = sdesc.shaderType;
}

ShaderBinary::~ShaderBinary() {}

BinaryData ShaderBinary::getData() const noexcept
{
	return BinaryData{m_blob->GetBufferPointer(), m_blob->GetBufferSize()};
}

ShaderType ShaderBinary::getType() const noexcept
{
	return m_type;
}
