#include <resources/PixelShader.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/resources/ShaderBinary.h>

using namespace genesis;
using namespace std;

PixelShader::PixelShader(const PixelShaderDesc& desc): Resource(desc.resource)
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
		ShaderType::PixelShader
	});

	m_pixelShader = desc.resource.graphicsDevice.createPixelShaderSignature({binary});
}

PixelShader::~PixelShader() {}

PixelShaderSignature& PixelShader::getSignature() noexcept
{
	return *m_pixelShader;
}