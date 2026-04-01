#include <graphics/resources/ShaderBinary.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <graphics/utils/GraphicsUtils.h>
#include <d3dcompiler.h>

using namespace genesis;
using namespace std;

ShaderBinary::ShaderBinary(const ShaderBinaryDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		D3DCreateBlob(sdesc.binaryCodeSize, &m_binary),
		"D3DCreateBlob failed."
	);
	memcpy(m_binary->GetBufferPointer(), sdesc.binaryCode, sdesc.binaryCodeSize);
}

ShaderBinary::ShaderBinary(const ShaderCompileDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!sdesc.sourceName) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source name provided.");
	}
	if (!sdesc.sourceCode) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source code provided.");
	}
	if (!sdesc.sourceCodeSize) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader source code size provided.");
	}
	if (!sdesc.entryPoint) {
		GENESIS_LOG_THROW_INVALID_ARG("No shader entry point provided.");
	}

	UINT compileFlags = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif

	GENESIS_GRAPHICS_CHECK_SHADER_COMPILE(
		D3DCompile(
			sdesc.sourceCode,
			sdesc.sourceCodeSize,
			sdesc.sourceName,
			nullptr,
			nullptr,
			sdesc.entryPoint,
			graphicsUtils::getShaderModelTarget(sdesc.type),
			compileFlags,
			0,
			&m_binary,
			&errorBlob
		),
		errorBlob.Get()
	);
}

ShaderBinary::~ShaderBinary() {}

BinaryData ShaderBinary::getData() const noexcept
{
	return BinaryData{m_binary->GetBufferPointer(), m_binary->GetBufferSize()};
}