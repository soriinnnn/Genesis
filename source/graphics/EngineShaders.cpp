#include <graphics/EngineShaders.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/shaders/fullscreenTriangle/vs.h>
#include <graphics/shaders/fullscreenTriangle/ps.h>

using namespace genesis;

EngineShaders::EngineShaders(const EngineShadersDesc& desc): Base(desc.base)
{
	m_fullscreenVS.binary = desc.graphicsDevice.createShaderBinary({fullscreenTriangleVS, sizeof(fullscreenTriangleVS)});
	m_fullscreenPS.binary = desc.graphicsDevice.createShaderBinary({fullscreenTrianglePS, sizeof(fullscreenTrianglePS)});
	m_fullscreenVS.signature = desc.graphicsDevice.reflectShader({*m_fullscreenVS.binary});
	m_fullscreenPS.signature = desc.graphicsDevice.reflectShader({*m_fullscreenPS.binary});
}

EngineShaders::~EngineShaders() {}

const ShaderBinary& EngineShaders::getFullscreenVSBinary() const
{
	return *m_fullscreenVS.binary;
}

const ShaderBinary& EngineShaders::getFullscreenPSBinary() const
{
	return *m_fullscreenPS.binary;
}

const ShaderSignature& EngineShaders::getFullscreenVSSignature() const
{
	return *m_fullscreenVS.signature;
}

const ShaderSignature& EngineShaders::getFullscreenPSSignature() const
{
	return *m_fullscreenPS.signature;
}