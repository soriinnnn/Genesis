#include <graphics/EngineStates.h>
#include <graphics/EngineShaders.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;

EngineStates::EngineStates(const EngineStatesDesc& desc): Base(desc.base), m_graphicsDevice{desc.graphicsDevice}
{
    auto& fullscreenShader = desc.engineShaders.fullscreenTriangle();
    m_frameBufferPipeline = m_graphicsDevice.createGraphicsPipelineState({fullscreenShader.vsBinary.get(), fullscreenShader.vsSignature.get(), fullscreenShader.psBinary.get(), fullscreenShader.psSignature.get()});

    auto& cubeSkyboxShader = desc.engineShaders.cubeSkybox();
    m_cubeSkyboxPipeline = m_graphicsDevice.createGraphicsPipelineState({cubeSkyboxShader.vsBinary.get(), cubeSkyboxShader.vsSignature.get(), cubeSkyboxShader.psBinary.get(), cubeSkyboxShader.psSignature.get()});

    m_pointWrap = m_graphicsDevice.createSamplerState({SamplerFilter::Point, SamplerAddressMode::Wrap, SamplerAddressMode::Wrap, SamplerAddressMode::Wrap});
    m_pointClamp = m_graphicsDevice.createSamplerState({SamplerFilter::Point, SamplerAddressMode::Clamp, SamplerAddressMode::Clamp, SamplerAddressMode::Clamp});
    m_bilinearWrap = m_graphicsDevice.createSamplerState({SamplerFilter::Bilinear, SamplerAddressMode::Wrap, SamplerAddressMode::Wrap, SamplerAddressMode::Wrap});
    m_bilinearClamp = m_graphicsDevice.createSamplerState({SamplerFilter::Bilinear, SamplerAddressMode::Clamp, SamplerAddressMode::Clamp, SamplerAddressMode::Clamp});
    m_trilinearWrap = m_graphicsDevice.createSamplerState({SamplerFilter::Trilinear, SamplerAddressMode::Wrap, SamplerAddressMode::Wrap, SamplerAddressMode::Wrap});
    m_trilinearClamp = m_graphicsDevice.createSamplerState({SamplerFilter::Trilinear, SamplerAddressMode::Clamp, SamplerAddressMode::Clamp, SamplerAddressMode::Clamp});

    m_depthDefault = m_graphicsDevice.createDepthStencilState({});
    m_depthDisabled = m_graphicsDevice.createDepthStencilState({false});
    m_depthSkybox = m_graphicsDevice.createDepthStencilState({true, false, ComparisonFunction::LessEqual});

    m_rasterizerSolid = m_graphicsDevice.createRasterizerState({});
    m_rasterizerSolidMSAA = m_graphicsDevice.createRasterizerState({true});
    m_rasterizerSkybox = m_graphicsDevice.createRasterizerState({false, CullMode::None});
}

EngineStates::~EngineStates() {}

const GraphicsPipelineState& EngineStates::frameBufferPipeline() const
{
    return *m_frameBufferPipeline;
}

const GraphicsPipelineState& EngineStates::cubeSkyboxPipeline() const
{
    return *m_cubeSkyboxPipeline;
}

const SamplerState& EngineStates::pointWrap() const
{
    return *m_pointWrap;
}

const SamplerState& EngineStates::pointClamp() const
{
    return *m_pointClamp;
}

const SamplerState& EngineStates::bilinearWrap() const
{
    return *m_bilinearWrap;
}

const SamplerState& EngineStates::bilinearClamp() const
{
    return *m_bilinearClamp;
}

const SamplerState& EngineStates::trilinearWrap() const
{
    return *m_trilinearWrap;
}

const SamplerState& EngineStates::trilinearClamp() const
{
    return *m_trilinearClamp;
}

const SamplerState& EngineStates::anisotropicWrap(uint32 maxAnisotropy) const
{
    SamplerStateDesc desc{};
    desc.filter = SamplerFilter::Anisotropic;
    desc.addressU = SamplerAddressMode::Wrap;
    desc.addressV = SamplerAddressMode::Wrap;
    desc.addressW = SamplerAddressMode::Wrap;
    desc.maxAnisotropy = maxAnisotropy;
    return *m_graphicsDevice.createSamplerState(desc);
}

const SamplerState& EngineStates::anisotropicClamp(uint32 maxAnisotropy) const
{
    SamplerStateDesc desc{};
    desc.filter = SamplerFilter::Anisotropic;
    desc.addressU = SamplerAddressMode::Clamp;
    desc.addressV = SamplerAddressMode::Clamp;
    desc.addressW = SamplerAddressMode::Clamp;
    desc.maxAnisotropy = maxAnisotropy;
    return *m_graphicsDevice.createSamplerState(desc);
}

const DepthStencilState& EngineStates::depthDefault() const
{
    return *m_depthDefault;
}

const DepthStencilState& EngineStates::depthDisabled() const
{
    return *m_depthDisabled;
}

const DepthStencilState& EngineStates::depthSkybox() const
{
    return *m_depthSkybox;
}

const RasterizerState& EngineStates::rasterizerSolid() const
{
    return *m_rasterizerSolid;
}

const RasterizerState& EngineStates::rasterizerSolidMSAA() const
{
    return *m_rasterizerSolidMSAA;
}

const RasterizerState& EngineStates::rasterizerSkybox() const
{
    return *m_rasterizerSkybox;
}
