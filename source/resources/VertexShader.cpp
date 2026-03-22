#include <resources/VertexShader.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/VertexShaderSignature.h>

using namespace genesis;
using namespace std;

VertexShader::VertexShader(const VertexShaderDesc& desc): Resource(desc.resource)
{
	string shaderData = resourcesUtils::readFile(m_path.c_str());
	if (shaderData.empty()) {
		GENESIS_LOG_THROW_ERROR("Failed to open shader file: {}", m_path.c_str());
	}

	SharedPtr<ShaderBinary> binary = desc.resource.graphicsDevice.compileShader({
		m_path.c_str(),
		shaderData.c_str(),
		shaderData.length(),
		desc.entry,
		ShaderType::VertexShader
	});

	m_vertexShader = desc.resource.graphicsDevice.createVertexShaderSignature({binary});
}

VertexShader::~VertexShader() {}

VertexShaderSignature& VertexShader::getSignature() noexcept
{
	return *m_vertexShader;
}