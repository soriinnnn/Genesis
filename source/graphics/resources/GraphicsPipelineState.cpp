#include <graphics/resources/GraphicsPipelineState.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/resources/VertexShaderSignature.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_DEPTH_STENCIL_DESC getDepthStencilDesc(const GraphicsPipelineStateDesc& desc);
static D3D_PRIMITIVE_TOPOLOGY getD3DPrimitiveTopology(PrimitiveTopology primitive);
static D3D11_COMPARISON_FUNC getD3D11ComparisonFunc(DepthComparison comparison);

GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& pdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (pdesc.pixelShader.getType() != ShaderType::PixelShader) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid pixel shader type.");
	}

	BinaryData vertexShader = pdesc.vertexShader.getShaderBinaryData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateVertexShader(
			vertexShader.m_data,
			vertexShader.dataSize,
			nullptr,
			&m_vertexShader
		),
		"CreateVertexShader failed."
	);

	BinaryData inputElements = pdesc.vertexShader.getInputElementsData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateInputLayout(
			static_cast<const D3D11_INPUT_ELEMENT_DESC*>(inputElements.m_data),
			static_cast<UINT>(inputElements.dataSize),
			vertexShader.m_data,
			vertexShader.dataSize,
			&m_inputLayout
		),
		"CreateInputLayout failed."
	);

	BinaryData pixelShader = pdesc.pixelShader.getData();
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreatePixelShader(
			pixelShader.m_data,
			pixelShader.dataSize,
			nullptr,
			&m_pixelShader
		),
		"CreatePixelShader failed."
	);

	D3D11_DEPTH_STENCIL_DESC depthDesc = getDepthStencilDesc(pdesc);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateDepthStencilState(
			&depthDesc, 
			&m_depthStencilState
		),
		"CreateDepthStencilState failed."
	);

	m_primitive = getD3DPrimitiveTopology(pdesc.primitive);
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
	depthDesc.DepthFunc = getD3D11ComparisonFunc(desc.depthComparison);

	return depthDesc;
}

static D3D_PRIMITIVE_TOPOLOGY getD3DPrimitiveTopology(PrimitiveTopology primitive) 
{
	switch (primitive) {
	case PrimitiveTopology::Points: 
		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	case PrimitiveTopology::Lines:
		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case PrimitiveTopology::LinesStrip:
		return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case PrimitiveTopology::Triangles:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveTopology::TrianglesStrip:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	default:
		return D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}

static D3D11_COMPARISON_FUNC getD3D11ComparisonFunc(DepthComparison comparison)
{
	switch (comparison) {
	case DepthComparison::Never:
		return D3D11_COMPARISON_NEVER;
	case DepthComparison::Less:
		return D3D11_COMPARISON_LESS;
	case DepthComparison::LessEqual:
		return D3D11_COMPARISON_LESS_EQUAL;
	case DepthComparison::Equal:
		return D3D11_COMPARISON_EQUAL;
	case DepthComparison::NotEqual:
		return D3D11_COMPARISON_NOT_EQUAL;
	case DepthComparison::Greater:
		return D3D11_COMPARISON_GREATER;
	case DepthComparison::GreaterEqual:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case DepthComparison::Always:
		return D3D11_COMPARISON_ALWAYS;
	default:
		return D3D11_COMPARISON_LESS;
	}
}