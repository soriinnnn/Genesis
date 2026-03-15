#include <resources/VertexShader.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/VertexShaderSignature.h>

using namespace genesis;
using namespace std;

VertexShader::VertexShader(const ResourceDesc& desc): Resource(desc) 
{
	size_t pos = m_path.find_last_of('@');
	if (pos == string::npos) {
		GENESIS_LOG_THROW_ERROR("Shader entry point not found.");
	}

	m_entryPoint = m_path.substr(pos + 1);
	m_path = m_path.substr(0, pos);
}

VertexShader::~VertexShader() {}

VertexShaderSignature& VertexShader::getVertexShaderSignature()
{
	if (!isLoaded()) {
		GENESIS_LOG_THROW_ERROR("Resource is not loaded: {}", m_path);
	}
	return *m_vertexShader;
}

void VertexShader::onLoad()
{
	string shaderData = resourcesUtils::readFile(m_path.c_str());
	if (shaderData.empty()) {
		GENESIS_LOG_THROW_ERROR("Failed to open shader file: {}", m_path);
	}

	SharedPtr<ShaderBinary> binary = m_graphicsDevice.compileShader({
		m_path.c_str(),
		shaderData.c_str(),
		shaderData.length(),
		m_entryPoint.c_str(),
		ShaderType::VertexShader
	});

	m_vertexShader = m_graphicsDevice.createVertexShaderSignature({binary});
}

void VertexShader::onUnload()
{
	m_vertexShader.reset();
}