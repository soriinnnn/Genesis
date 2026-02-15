#include <graphics/GraphicsPipelineState.h>
#include <graphics/ShaderBinary.h>
#include <graphics/GraphicsLogUtils.h>

using namespace genesis;

inline constexpr D3D11_INPUT_ELEMENT_DESC layoutElements[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

// --------------------------------------------------------------------------------

GraphicsPipelineState::GraphicsPipelineState(const GraphicsPipelineStateDesc& gpDesc, const GraphicsResourceDesc& grDesc): GraphicsResource(grDesc)
{
	if (gpDesc.vertexShader.getType() != ShaderType::VertexShader) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid vertex shader type.");
	}
	if (gpDesc.pixelShader.getType() != ShaderType::PixelShader) {
		GENESIS_LOG_THROW_INVALID_ARG("Invalid pixel shader type.");
	}

	ShaderBinaryData vertexShader = gpDesc.vertexShader.getData();
	ShaderBinaryData pixelShader = gpDesc.pixelShader.getData();

	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateInputLayout(
			layoutElements,
			std::size(layoutElements),
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