#include <graphics/resources/GraphicsPipelineState.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/resources/ShaderSignature.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& pdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (!pdesc.vsBinary) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex shader binary is null.");
	}
	if (!pdesc.vsSignature) {
		GENESIS_LOG_THROW_INVALID_ARG("Vertex shader signature is null.");
	}
	if (!pdesc.psBinary) {
		GENESIS_LOG_THROW_INVALID_ARG("Pixel shader binary is null.");
	}
	if (!pdesc.psSignature) {
		GENESIS_LOG_THROW_INVALID_ARG("Pixel shader signature is null.");
	}

	BinaryData vertexShader = pdesc.vsBinary->getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateVertexShader(
			vertexShader.data,
			vertexShader.size,
			nullptr,
			&m_vertexShader
		),
		"CreateVertexShader failed."
	);

	BinaryData pixelShader = pdesc.psBinary->getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreatePixelShader(
			pixelShader.data,
			pixelShader.size,
			nullptr,
			&m_pixelShader
		),
		"CreatePixelShader failed."
	);

	BinaryData inputElements = pdesc.vsSignature->getInputElementsData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateInputLayout(
			static_cast<const D3D11_INPUT_ELEMENT_DESC*>(inputElements.data),
			static_cast<UINT>(inputElements.size),
			vertexShader.data,
			vertexShader.size,
			&m_inputLayout
		),
		"CreateInputLayout failed."
	);

	m_primitive = graphicsUtils::getD3DPrimitiveTopology(pdesc.primitive);
	if (m_primitive == D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED) {
		GENESIS_LOG_THROW_ERROR("Invalid primitive topology.");
	}
}

GraphicsPipelineState::~GraphicsPipelineState() {}