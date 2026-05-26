#include <graphics/resources/RasterizerState.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_RASTERIZER_DESC getD3D11RasterizerDesc(bool multiSampling);

RasterizerState::RasterizerState(const RasterizerStateDesc& rdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	D3D11_RASTERIZER_DESC rasterizerDesc = getD3D11RasterizerDesc(rdesc.multiSampling);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateRasterizerState(
			&rasterizerDesc,
			&m_rasterizerState
		),
		"CreateRasterizerState failed."
	);
}

RasterizerState::~RasterizerState() {}

/* STATIC FUNCTION DEFINITIONS */

D3D11_RASTERIZER_DESC getD3D11RasterizerDesc(bool multiSampling)
{
	D3D11_RASTERIZER_DESC desc{};

	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = true;
	desc.ScissorEnable = false;
	desc.MultisampleEnable = multiSampling;
	desc.AntialiasedLineEnable = multiSampling;

	return desc;
}