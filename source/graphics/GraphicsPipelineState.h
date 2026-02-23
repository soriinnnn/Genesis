#ifndef GENESIS_GRAPHICS_PIPELINE_STATE_H
#define GENESIS_GRAPHICS_PIPELINE_STATE_H
#include <graphics/GraphicsResource.h>
#include <graphics/GraphicsTypes.h>

namespace genesis
{
	struct GraphicsPipelineStateDesc
	{
		const VertexShaderSignature& vertexShader;
		const ShaderBinary& pixelShader;
		PrimitiveTopology primitive = PrimitiveTopology::Triangles;
	};

	class GraphicsPipelineState final: public GraphicsResource
	{
	public:
		GraphicsPipelineState(const GraphicsPipelineStateDesc& gpDesc, const GraphicsResourceDesc& grDesc);
		~GraphicsPipelineState();
	
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		D3D_PRIMITIVE_TOPOLOGY m_primitive;

		friend class DeviceContext;
	};
}

#endif