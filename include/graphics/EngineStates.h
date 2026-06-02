#ifndef GENESIS_ENGINE_STATES_H
#define GENESIS_ENGINE_STATES_H
#include <core/Base.h>
#include <core/Core.h>

GENESIS_NAMESPACE_BEGIN

struct EngineStatesDesc
{
	BaseDesc base;
	GraphicsDevice& graphicsDevice;
	EngineShaders& engineShaders;
};

class EngineStates final: public Base
{
public:
	explicit EngineStates(const EngineStatesDesc& desc);
	~EngineStates() override;

	const GraphicsPipelineState& frameBufferPipeline() const;
	const GraphicsPipelineState& cubeSkyboxPipeline() const;

	const SamplerState& pointWrap() const;
	const SamplerState& pointClamp() const;
	const SamplerState& bilinearWrap() const;
	const SamplerState& bilinearClamp() const;
	const SamplerState& trilinearWrap() const;
	const SamplerState& trilinearClamp() const;
	const SamplerState& anisotropicWrap(uint32 maxAnisotropy) const;
	const SamplerState& anisotropicClamp(uint32 maxAnisotropy) const;

	const DepthStencilState& depthDefault() const;
	const DepthStencilState& depthDisabled() const;
	const DepthStencilState& depthSkybox() const;

	const RasterizerState& rasterizerSolid() const;
	const RasterizerState& rasterizerSolidMSAA() const;
	const RasterizerState& rasterizerSkybox() const;

private:
	GraphicsDevice& m_graphicsDevice;
	SharedPtr<GraphicsPipelineState> m_frameBufferPipeline;
	SharedPtr<GraphicsPipelineState> m_cubeSkyboxPipeline;
	SharedPtr<SamplerState> m_pointWrap;
	SharedPtr<SamplerState> m_pointClamp;
	SharedPtr<SamplerState> m_bilinearWrap;
	SharedPtr<SamplerState> m_bilinearClamp;
	SharedPtr<SamplerState> m_trilinearWrap;
	SharedPtr<SamplerState> m_trilinearClamp;
	SharedPtr<DepthStencilState> m_depthDefault;
	SharedPtr<DepthStencilState> m_depthDisabled;
	SharedPtr<DepthStencilState> m_depthSkybox;
	SharedPtr<RasterizerState> m_rasterizerSolid;
	SharedPtr<RasterizerState> m_rasterizerSolidMSAA;
	SharedPtr<RasterizerState> m_rasterizerSkybox;
};

GENESIS_NAMESPACE_END

#endif