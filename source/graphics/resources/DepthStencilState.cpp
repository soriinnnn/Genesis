#include <graphics/resources/DepthStencilState.h>
#include <graphics/utils/GraphicsUtils.h>
#include <graphics/utils/GraphicsLogUtils.h>

using namespace genesis;
using namespace graphicsUtils;

static D3D11_DEPTH_STENCIL_DESC getD3D11DepthStencilDesc(bool depthEnable, bool depthWrite, ComparisonFunction depthComparison, bool stencilEnable, StencilFaceBehaviour frontFace, StencilFaceBehaviour backFace);

DepthStencilState::DepthStencilState(const DepthStencilStateDesc& sdesc, const GraphicsResourceDesc& gdesc): GraphicsResource(gdesc)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = getD3D11DepthStencilDesc(sdesc.depthEnable, sdesc.depthWrite, sdesc.depthComparison, sdesc.stencilEnable, sdesc.frontFace, sdesc.backFace);
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

static D3D11_DEPTH_STENCIL_DESC getD3D11DepthStencilDesc(bool depthEnable, bool depthWrite, ComparisonFunction depthComparison, bool stencilEnable, StencilFaceBehaviour frontFace, StencilFaceBehaviour backFace)
{
	D3D11_DEPTH_STENCIL_DESC desc{};

	desc.DepthEnable = depthEnable;
	desc.DepthWriteMask = (depthWrite) ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = getD3D11ComparisonFunc(depthComparison);
	desc.StencilEnable = stencilEnable;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	desc.FrontFace.StencilFailOp = getD3D11StencilOP(frontFace.stencilFail);
	desc.FrontFace.StencilDepthFailOp = getD3D11StencilOP(frontFace.stencilDepthFail);
	desc.FrontFace.StencilPassOp = getD3D11StencilOP(frontFace.stencilPass);
	desc.FrontFace.StencilFunc = getD3D11ComparisonFunc(frontFace.stencilComparison);
	desc.BackFace.StencilFailOp = getD3D11StencilOP(backFace.stencilFail);
	desc.BackFace.StencilDepthFailOp = getD3D11StencilOP(backFace.stencilDepthFail);
	desc.BackFace.StencilPassOp = getD3D11StencilOP(backFace.stencilPass);
	desc.BackFace.StencilFunc = getD3D11ComparisonFunc(backFace.stencilComparison);

	return desc;
}