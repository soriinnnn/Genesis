#include <graphics/EngineStates.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;

EngineStates::EngineStates(const EngineStatesDesc& desc): Base(desc.base) 
{
    m_pointClamp = desc.graphicsDevice.createSamplerState({
        TextureFilter::Point,
        TextureAddressMode::Clamp,
        TextureAddressMode::Clamp,
        TextureAddressMode::Clamp
    });
    m_pointWrap = desc.graphicsDevice.createSamplerState({
        TextureFilter::Point,
        TextureAddressMode::Wrap,
        TextureAddressMode::Wrap,
        TextureAddressMode::Wrap
    });
    m_linearClamp = desc.graphicsDevice.createSamplerState({
        TextureFilter::Linear,
        TextureAddressMode::Clamp,
        TextureAddressMode::Clamp,
        TextureAddressMode::Clamp
    });
    m_linearWrap = desc.graphicsDevice.createSamplerState({
        TextureFilter::Linear,
        TextureAddressMode::Wrap,
        TextureAddressMode::Wrap,
        TextureAddressMode::Wrap
    });
}

EngineStates::~EngineStates() {}

const SamplerState& EngineStates::getPointClamp() const
{
    return *m_pointClamp;
}

const SamplerState& EngineStates::getPointWrap() const
{
    return *m_pointWrap;
}

const SamplerState& EngineStates::getLinearClamp() const
{
    return *m_linearClamp;
}

const SamplerState& EngineStates::getLinearWrap() const
{
    return *m_linearWrap;
}
