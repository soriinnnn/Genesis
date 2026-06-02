#ifndef GENESIS_RASTERIZER_STATE_H
#define GENESIS_RASTERIZER_STATE_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsUtils.h>

GENESIS_NAMESPACE_BEGIN

struct RasterizerStateDesc
{
	bool multiSampling = false;
	CullMode cullMode = CullMode::Back;
};

class RasterizerState final: public GraphicsResource
{
public:
	RasterizerState(const RasterizerStateDesc& rdesc, const GraphicsResourceDesc& gdesc);
	~RasterizerState() override;

private:
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;

	friend class DeviceContext;
};

GENESIS_NAMESPACE_END

#endif