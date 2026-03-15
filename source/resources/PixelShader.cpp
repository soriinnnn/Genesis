#include <resources/PixelShader.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/ShaderBinary.h>

using namespace genesis;
using namespace std;

PixelShader::PixelShader(const ResourceDesc& desc): Resource(desc) 
{
	size_t pos = m_path.find_last_of('@');
	if (pos == string::npos) {
		GENESIS_LOG_THROW_ERROR("Shader entry point not found.");
	}

	m_entryPoint = m_path.substr(pos + 1);
	m_path = m_path.substr(0, pos);
}

PixelShader::~PixelShader() {}

ShaderBinary& PixelShader::getShaderBinary()
{
	if (!isLoaded()) {
		GENESIS_LOG_THROW_ERROR("Resource is not loaded: {}", m_path);
	}
	return *m_pixelShader;
}

void PixelShader::onLoad()
{
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

void PixelShader::onUnload()
{
	m_pixelShader.reset();
}