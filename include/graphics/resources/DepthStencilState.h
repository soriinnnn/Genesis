#ifndef GENESIS_DEPTH_STENCIL_STATE_H
#define GENESIS_DEPTH_STENCIL_STATE_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>

GENESIS_NAMESPACE_BEGIN

struct DepthStencilStateDesc
{
	bool depthEnable = true;
	bool depthWrite = true;
	ComparisonFunction depthComparison = ComparisonFunction::Less;
	bool stencilEnable = false;
	StencilFaceBehaviour frontFace;
	StencilFaceBehaviour backFace;
};

class DepthStencilState final: public GraphicsResource
{
public:
	DepthStencilState(const DepthStencilStateDesc& sdesc, const GraphicsResourceDesc& gdesc);
	~DepthStencilState() override;

private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	friend class DeviceContext;
};

GENESIS_NAMESPACE_END

#endif