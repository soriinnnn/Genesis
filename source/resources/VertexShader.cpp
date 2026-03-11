#include <resources/VertexShader.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/VertexShaderSignature.h>

using namespace genesis;
using namespace std;

VertexShader::VertexShader(const VertexShaderDesc& vdesc, const ResourceDesc& rdesc): Resource(rdesc), m_graphicsDevice(vdesc.graphicsDevice), m_entryPoint(vdesc.entryPoint) {}

VertexShader::~VertexShader() {}

VertexShaderSignature& VertexShader::getVertexShaderSignature()
{
	if (!m_vertexShader) {
		GENESIS_LOG_THROW_ERROR("Shader is not loaded.\nResource: {}", m_path);
	}
	return *m_vertexShader;
}

void VertexShader::load()
{
	if (m_vertexShader) {
		GENESIS_LOG_WARNING("Shader is already loaded.\nResource: {}", m_path);
		return;
	}

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

void VertexShader::unload()
{
	m_vertexShader.reset();
}
