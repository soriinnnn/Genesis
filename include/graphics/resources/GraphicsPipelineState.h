#ifndef GENESIS_GRAPHICS_PIPELINE_STATE_H
#define GENESIS_GRAPHICS_PIPELINE_STATE_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>

namespace genesis
{
	struct GraphicsPipelineStateDesc
	{
		const ShaderBinary& vsBinary;
		const ShaderBinary& psBinary;
		const ShaderSignature& vsSignature;
		const ShaderSignature& psSignature;
		PrimitiveTopology primitive = PrimitiveTopology::Triangles;
		bool depthEnable = true;
		ComparisonFunction depthComparison = ComparisonFunction::Less;
	};

	class GraphicsPipelineState final: public GraphicsResource
	{
	public:
		GraphicsPipelineState(const GraphicsPipelineStateDesc& pdesc, const GraphicsResourceDesc& gdesc);
		~GraphicsPipelineState() override;
	
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		D3D_PRIMITIVE_TOPOLOGY m_primitive;

		friend class DeviceContext;
	};
}

#endif