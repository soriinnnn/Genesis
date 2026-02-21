#include <graphics/GraphicsPipelineState.h>
#include <graphics/ShaderBinary.h>
#include <graphics/VertexShaderSignature.h>
#include <graphics/GraphicsLogUtils.h>

using namespace genesis;

GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& gpDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (gpDesc.pixelShader.getType() != ShaderType::PixelShader) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid pixel shader type.");
	}

	BinaryData vertexShader = gpDesc.vertexShader.getShaderBinaryData();
	BinaryData pixelShader = gpDesc.pixelShader.getData();
	BinaryData inputElements = gpDesc.vertexShader.getInputElementsData();

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateInputLayout(
			static_cast<const D3D11_INPUT_ELEMENT_DESC*>(inputElements.data),
			static_cast<UINT>(inputElements.dataSize),
			vertexShader.data,
			vertexShader.dataSize,
			&m_inputLayout
		),
		"CreateInputLayout failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateVertexShader(
			vertexShader.data,
			vertexShader.dataSize,
			nullptr,
			&m_vertexShader
		),
		"CreateVertexShader failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreatePixelShader(
			pixelShader.data,
			pixelShader.dataSize,
			nullptr,
			&m_pixelShader
		),
		"CreatePixelShader failed."
	);
}

GraphicsPipelineState::~GraphicsPipelineState() {}