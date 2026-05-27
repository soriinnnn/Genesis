#ifndef GENESIS_GRAPHICS_PIPELINE_STATE_H
#define GENESIS_GRAPHICS_PIPELINE_STATE_H
#include <graphics/resources/GraphicsResource.h>
#include <graphics/utils/GraphicsTypes.h>

GENESIS_NAMESPACE_BEGIN

struct GraphicsPipelineStateDesc
{
	const ShaderBinary* vsBinary;
	const ShaderSignature* vsSignature;
	const ShaderBinary* psBinary;
	const ShaderSignature* psSignature;
	PrimitiveTopology primitive = PrimitiveTopology::Triangles;
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
	D3D_PRIMITIVE_TOPOLOGY m_primitive;

	friend class DeviceContext;
};

GENESIS_NAMESPACE_END

#endif