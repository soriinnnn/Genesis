#ifndef GENESIS_RASTERIZER_STATE_H
#define GENESIS_RASTERIZER_STATE_H
#include <graphics/resources/GraphicsResource.h>

namespace genesis
{
	struct RasterizerStateDesc
	{
		bool multiSampling = false;
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
}

#endif