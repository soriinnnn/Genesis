#include <resources/Shader.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>

using namespace genesis;
using namespace std;

Shader::Shader(const ShaderDesc& desc): Resource(desc.resource), m_type{desc.type}
{
	auto& graphicsContext = desc.resource.graphicsContext;

	string data = resourcesUtils::readFile(m_path.c_str());
	if (data.empty()) {
		GENESIS_LOG_THROW_ERROR("Failed to open shader file: {}", m_path.c_str());
	}

	m_binary = graphicsContext.graphicsDevice.compileShader({
		m_path.c_str(),
		data.c_str(),
		data.length(),
		desc.entry,
		desc.type
	});
	
	m_signature = graphicsContext.graphicsDevice.reflectShader({*m_binary});
}

Shader::~Shader() {}

ShaderType Shader::getType() const noexcept
{
	return m_type;
}

ShaderBinary& Shader::getBinary() noexcept
{
	return *m_binary;
}

ShaderSignature& Shader::getSignature() noexcept
{
	return *m_signature;
}