#include <graphics/EngineShaders.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/shaders/fullscreenTriangle/vs.h>
#include <graphics/shaders/fullscreenTriangle/ps.h>
#include <graphics/shaders/debugLine/vs.h>
#include <graphics/shaders/debugLine/ps.h>
#include <graphics/shaders/sphereSkybox/vs.h>
#include <graphics/shaders/sphereSkybox/ps.h>
#include <graphics/shaders/cubeSkybox/vs.h>
#include <graphics/shaders/cubeSkybox/ps.h>

using namespace genesis;

static EngineShaders::EngineShader createShader(const void* vsCompiledCode, size_t vsSize, const void* psCompiledCode, size_t psSize, GraphicsDevice& graphicsDevice);

EngineShaders::EngineShaders(const EngineShadersDesc& desc): Base(desc.base)
{
	m_fullscreenTriangle = createShader(fullscreenTriangleVS, sizeof(fullscreenTriangleVS), fullscreenTrianglePS, sizeof(fullscreenTrianglePS), desc.graphicsDevice);
	m_debugLine = createShader(debugLineVS, sizeof(debugLineVS), debugLinePS, sizeof(debugLinePS), desc.graphicsDevice);
	m_sphereSkybox = createShader(sphereSkyboxVS, sizeof(sphereSkyboxVS), sphereSkyboxPS, sizeof(sphereSkyboxPS), desc.graphicsDevice);
	m_cubeSkybox = createShader(cubeSkyboxVS, sizeof(cubeSkyboxVS), cubeSkyboxPS, sizeof(cubeSkyboxPS), desc.graphicsDevice);
}

EngineShaders::~EngineShaders() {}

const EngineShaders::EngineShader& EngineShaders::fullscreenTriangle() const
{
	return m_fullscreenTriangle;
}

const EngineShaders::EngineShader& EngineShaders::debugLine() const
{
	return m_debugLine;
}

const EngineShaders::EngineShader& EngineShaders::sphereSkybox() const
{
	return m_sphereSkybox;
}

const EngineShaders::EngineShader& EngineShaders::cubeSkybox() const
{
	return m_cubeSkybox;
}

/* STATIC FUNCTIONS DEFINITIONS */

EngineShaders::EngineShader createShader(const void* vsCompiledCode, size_t vsSize, const void* psCompiledCode, size_t psSize, GraphicsDevice& graphicsDevice)
{
	EngineShaders::EngineShader shader;

	shader.vsBinary = graphicsDevice.createShaderBinary({vsCompiledCode, vsSize});
	shader.psBinary = graphicsDevice.createShaderBinary({psCompiledCode, psSize});
	shader.vsSignature = graphicsDevice.reflectShader({*shader.vsBinary});
	shader.psSignature = graphicsDevice.reflectShader({*shader.psBinary});

	return shader;
}