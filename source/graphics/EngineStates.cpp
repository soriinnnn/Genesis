#include <graphics/EngineStates.h>
#include <graphics/EngineShaders.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;

EngineStates::EngineStates(const EngineStatesDesc& desc): Base(desc.base), m_graphicsDevice{desc.graphicsDevice}
{
    auto& fullscreenShader = desc.engineShaders.fullscreenTriangle();
    m_frameBufferPipeline = m_graphicsDevice.createGraphicsPipelineState({fullscreenShader.vsBinary.get(), fullscreenShader.vsSignature.get(), fullscreenShader.psBinary.get(), fullscreenShader.psSignature.get()});
    m_pointWrap = m_graphicsDevice.createSamplerState({TextureFilter::Point, TextureAddressMode::Wrap, TextureAddressMode::Wrap, TextureAddressMode::Wrap});
    m_pointClamp = m_graphicsDevice.createSamplerState({TextureFilter::Point, TextureAddressMode::Clamp, TextureAddressMode::Clamp, TextureAddressMode::Clamp});
    m_bilinearWrap = m_graphicsDevice.createSamplerState({TextureFilter::Bilinear, TextureAddressMode::Wrap, TextureAddressMode::Wrap, TextureAddressMode::Wrap});
    m_bilinearClamp = m_graphicsDevice.createSamplerState({TextureFilter::Bilinear, TextureAddressMode::Clamp, TextureAddressMode::Clamp, TextureAddressMode::Clamp});
    m_trilinearWrap = m_graphicsDevice.createSamplerState({TextureFilter::Trilinear, TextureAddressMode::Wrap, TextureAddressMode::Wrap, TextureAddressMode::Wrap});
    m_trilinearClamp = m_graphicsDevice.createSamplerState({TextureFilter::Trilinear, TextureAddressMode::Clamp, TextureAddressMode::Clamp, TextureAddressMode::Clamp});
    m_depthDefault = m_graphicsDevice.createDepthStencilState({});
    m_depthDefault = m_graphicsDevice.createDepthStencilState({false});
    m_rasterizerSolid = m_graphicsDevice.createRasterizerState({});
    m_rasterizerSolidMSAA = m_graphicsDevice.createRasterizerState({true});
}

EngineStates::~EngineStates() {}

const GraphicsPipelineState& EngineStates::frameBufferPipeline() const
{
    return *m_frameBufferPipeline;
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
    desc.filter = TextureFilter::Anisotropic;
    desc.addressU = TextureAddressMode::Wrap;
    desc.addressV = TextureAddressMode::Wrap;
    desc.addressW = TextureAddressMode::Wrap;
    desc.maxAnisotropy = maxAnisotropy;
    return *m_graphicsDevice.createSamplerState(desc);
}

const SamplerState& EngineStates::anisotropicClamp(uint32 maxAnisotropy) const
{
    SamplerStateDesc desc{};
    desc.filter = TextureFilter::Anisotropic;
    desc.addressU = TextureAddressMode::Clamp;
    desc.addressV = TextureAddressMode::Clamp;
    desc.addressW = TextureAddressMode::Clamp;
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

const RasterizerState& EngineStates::rasterizerSolid() const
{
    return *m_rasterizerSolid;
}

const RasterizerState& EngineStates::rasterizerSolidMSAA() const
{
    return *m_rasterizerSolidMSAA;
}