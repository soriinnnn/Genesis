#include <graphics/resources/GraphicsPipelineState.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/resources/ShaderSignature.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_DEPTH_STENCIL_DESC getD3D11DepthStencilDesc(bool depthEnable, ComparisonFunction depthComparison);

GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& pdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	BinaryData vertexShader = pdesc.vsBinary.getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateVertexShader(
			vertexShader.data,
			vertexShader.size,
			nullptr,
			&m_vertexShader
		),
		"CreateVertexShader failed."
	);

	BinaryData pixelShader = pdesc.psBinary.getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreatePixelShader(
			pixelShader.data,
			pixelShader.size,
			nullptr,
			&m_pixelShader
		),
		"CreatePixelShader failed."
	);

	BinaryData inputElements = pdesc.vsSignature.getInputElementsData();
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

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = getD3D11DepthStencilDesc(pdesc.depthEnable, pdesc.depthComparison);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateDepthStencilState(
			&depthStencilDesc, 
			&m_depthStencilState
		),
		"CreateDepthStencilState failed."
	);

	m_primitive = graphicsUtils::getD3DPrimitiveTopology(pdesc.primitive);
	if (m_primitive == D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED) {
		GENESIS_LOG_THROW_ERROR("Invalid primitive topology.");
	}
}

GraphicsPipelineState::~GraphicsPipelineState() {}

/* STATIC FUNCTION DEFINITIONS */

static D3D11_DEPTH_STENCIL_DESC getD3D11DepthStencilDesc(bool depthEnable, ComparisonFunction depthComparison)
{
	D3D11_DEPTH_STENCIL_DESC desc{};

	desc.DepthEnable = depthEnable;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = graphicsUtils::getD3D11ComparisonFunc(depthComparison);
	desc.StencilEnable = false;
	desc.StencilReadMask = 0;
	desc.StencilWriteMask = 0;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	return desc;
}