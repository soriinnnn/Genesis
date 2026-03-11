#include <resources/PixelShader.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/ShaderBinary.h>

using namespace genesis;
using namespace std;

PixelShader::PixelShader(const PixelShaderDesc& sdesc, const ResourceDesc& rdesc): Resource(rdesc), m_graphicsDevice(sdesc.graphicsDevice), m_entryPoint{sdesc.entryPoint} {}

PixelShader::~PixelShader() {}

ShaderBinary& PixelShader::getShaderBinary()
{
	if (!m_pixelShader) {
		GENESIS_LOG_THROW_ERROR("Shader is not loaded.\nResource: {}", m_path);
	}
	return *m_pixelShader;
}

void PixelShader::load()
{
	if (m_pixelShader) {
		GENESIS_LOG_WARNING("Shader is already loaded.\nResource: {}", m_path);
		return;
	}

	string shaderData = resourcesUtils::readFile(m_path.c_str());
	if (shaderData.empty()) {
		GENESIS_LOG_THROW_ERROR("Failed to open shader file: {}", m_path);
	}

	m_pixelShader = m_graphicsDevice.compileShader({
		m_path.c_str(),
		shaderData.c_str(),
		shaderData.length(),
		m_entryPoint.c_str(),
		ShaderType::PixelShader
	});
}

void PixelShader::unload()
{
	m_pixelShader.reset();
}