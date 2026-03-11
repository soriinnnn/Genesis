#include <graphics/resources/GraphicsPipelineState.h>
#include <graphics/resources/ShaderBinary.h>
#include <graphics/resources/VertexShaderSignature.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D_PRIMITIVE_TOPOLOGY getD3DPrimitiveTopology(PrimitiveTopology primitive);

GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& pdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	if (pdesc.pixelShader.getType() != ShaderType::PixelShader) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid pixel shader type.");
	}

	BinaryData vertexShader = pdesc.vertexShader.getShaderBinaryData();
	BinaryData pixelShader = pdesc.pixelShader.getData();
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

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateVertexShader(
			vertexShader.m_data,
			vertexShader.dataSize,
			nullptr,
			&m_vertexShader
		),
		"CreateVertexShader failed."
	);

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreatePixelShader(
			pixelShader.m_data,
			pixelShader.dataSize,
			nullptr,
			&m_pixelShader
		),
		"CreatePixelShader failed."
	);

	m_primitive = getD3DPrimitiveTopology(pdesc.primitive);
	if (m_primitive == D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED) {
		GENESIS_LOG_THROW_ERROR("Invalid primitive topology");
	}
}

GraphicsPipelineState::~GraphicsPipelineState() {}

/* STATIC FUNCTION DEFINITIONS */

static D3D_PRIMITIVE_TOPOLOGY getD3DPrimitiveTopology(PrimitiveTopology primitive) {
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