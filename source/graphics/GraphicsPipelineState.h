#ifndef GENESIS_GRAPHICS_PIPELINE_STATE_H
#define GENESIS_GRAPHICS_PIPELINE_STATE_H
#include <graphics/GraphicsResource.h>

namespace genesis
{
	class GraphicsPipelineState final: public GraphicsResource
	{
	public: 
		GraphicsPipelineState(const GraphicsPipelineStateDesc& gpDesc, const GraphicsResourceDesc& grDesc);
		~GraphicsPipelineState();

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		friend class DeviceContext;
	};
}

#endif