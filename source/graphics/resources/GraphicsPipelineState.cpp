#include <graphics/resources/GraphicsPipelineState.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/resources/ShaderSignature.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_DEPTH_STENCIL_DESC getDepthStencilDesc(const GraphicsPipelineStateDesc& desc);

GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& pdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	BinaryData vertexShader = pdesc.vertexShaderBinary.getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateVertexShader(
			vertexShader.data,
			vertexShader.dataSize,
			nullptr,
			&m_vertexShader
		),
		"CreateVertexShader failed."
	);

	BinaryData pixelShader = pdesc.pixelShaderBinary.getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreatePixelShader(
			pixelShader.data,
			pixelShader.dataSize,
			nullptr,
			&m_pixelShader
		),
		"CreatePixelShader failed."
	);

	BinaryData inputElements = pdesc.vertexShaderSignature.getInputElementsData();
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

	D3D11_DEPTH_STENCIL_DESC depthDesc = getDepthStencilDesc(pdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateDepthStencilState(
			&depthDesc, 
			&m_depthStencilState
		),
		"CreateDepthStencilState failed."
	);

	m_primitive = graphicsUtils::getD3DPrimitiveTopology(pdesc.primitive);
	if (m_primitive == D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED) {
		GENESIS_LOG_THROW_ERROR("Invalid primitive topology");
	}
}

GraphicsPipelineState::~GraphicsPipelineState() {}

/* STATIC FUNCTION DEFINITIONS */

static D3D11_DEPTH_STENCIL_DESC getDepthStencilDesc(const GraphicsPipelineStateDesc& desc)
{
	D3D11_DEPTH_STENCIL_DESC depthDesc{};

	depthDesc.DepthEnable = desc.depthEnable;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = graphicsUtils::getD3D11ComparisonFunc(desc.depthComparison);

	return depthDesc;
}