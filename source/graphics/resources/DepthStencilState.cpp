#include <graphics/resources/DepthStencilState.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;

static D3D11_DEPTH_STENCIL_DESC getD3D11DepthStencilDesc(bool depthEnable, ComparisonFunction depthComparison);

DepthStencilState::DepthStencilState(const DepthStencilStateDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = getD3D11DepthStencilDesc(sdesc.depthEnable, sdesc.depthComparison);
	GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(
		m_device.CreateDepthStencilState(
			&depthStencilDesc,
			&m_depthStencilState
		),
		"CreateDepthStencilState failed."
	);
}

DepthStencilState::~DepthStencilState() {}

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