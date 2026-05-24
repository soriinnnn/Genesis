#include <graphics/resources/ShaderBinary.h>
#include <graphics/utils/GraphicsLogUtils.h>
#include <graphics/utils/GraphicsUtils.h>
#include <d3dcompiler.h>

using namespace genesis;
using namespace std;

ShaderBinary::ShaderBinary(const ShaderBinaryDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!sdesc.binaryCode || !sdesc.binaryCodeSize) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid shader binary data.");
	}
		
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		D3DCreateBlob(sdesc.binaryCodeSize, &m_binary),
		"D3DCreateBlob failed."
	);
	memcpy(m_binary->GetBufferPointer(), sdesc.binaryCode, sdesc.binaryCodeSize);
}

ShaderBinary::ShaderBinary(const ShaderCompileDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!sdesc.name) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid shader source name.");
	}
	if (!sdesc.code) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid shader source code.");
	}
	if (!sdesc.codeSize) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid shader source code size.");
	}
	if (!sdesc.entryPoint) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid shader entry point.");
	}

	UINT flags = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif

	GENESIS_GRAPHICS_CHECK_SHADER_COMPILE(
		D3DCompile(
			sdesc.code,
			sdesc.codeSize,
			sdesc.name,
			nullptr,
			nullptr,
			sdesc.entryPoint,
			graphicsUtils::getShaderModelTarget(sdesc.type),
			flags,
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