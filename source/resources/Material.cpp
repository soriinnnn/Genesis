#include <resources/Material.h>
#include <resources/Texture.h>
#include <resources/Shader.h>
#include <resources/ResourceManager.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/utils/GraphicsMacros.h>
#include <nlohmann/json.hpp>
#include <cmath>

using namespace genesis;
using namespace std;
using namespace nlohmann;

static SharedPtr<Shader> getShader(json& data, ShaderType type, ResourceManager& resourceManager);
static vector<uint8> getPropertiesValues(json& data, const ShaderReflectionConstantBuffer& cbuffer, const char* path, Logger& logger);

Material::Material(const MaterialDesc& desc): Resource(desc.resource)
{
	try {
		json data = json::parse(resourcesUtils::readFile(m_path.c_str()));

		m_vertexShader = getShader(data, ShaderType::VertexShader, desc.resource.resourceManager);
		m_pixelShader = getShader(data, ShaderType::PixelShader, desc.resource.resourceManager);
		m_pipeline = desc.resource.graphicsDevice.createGraphicsPipelineState({
			m_vertexShader->getBinary(), 
			m_pixelShader->getBinary(), 
			m_vertexShader->getSignature(), 
			m_pixelShader->getSignature()
		});

		if (data.contains("textures")) {
			for (auto& texture : data["textures"]) {
				string path = texture.at("path");
				uint32 slot = texture.at("slot");
				m_textures.push_back({desc.resource.resourceManager.getTexture(path.c_str()), slot});
			}
		}
		
		if (data.contains("properties")) {
			const ShaderReflectionConstantBuffer* cbuffer = nullptr;
			auto& vsSignature = m_vertexShader->getSignature();
			auto& psSignature = m_pixelShader->getSignature();
			
			if (vsSignature.hasConstantBuffer(MATERIAL_CONSTANT_BUFFER_NAME)) {
				cbuffer = vsSignature.getConstantBufferReflection(MATERIAL_CONSTANT_BUFFER_NAME);
			}
			else if (psSignature.hasConstantBuffer(MATERIAL_CONSTANT_BUFFER_NAME)) {
				cbuffer = psSignature.getConstantBufferReflection(MATERIAL_CONSTANT_BUFFER_NAME);
			}
			else {
				GENESIS_LOG_WARNING("Material \"{}\" has properties defined but no \"{}\" constant buffer found in shaders, skipping.", m_path.c_str(), MATERIAL_CONSTANT_BUFFER_NAME);
			}

			if (cbuffer) {
				vector<uint8> buffer = getPropertiesValues(data, *cbuffer, m_path.c_str(), getLogger());
				m_properties = desc.resource.graphicsDevice.createConstantBuffer({buffer.data(), static_cast<uint32>(buffer.size())});

				if (cbuffer->slot != MATERIAL_CONSTANT_BUFFER_SLOT) {
					GENESIS_LOG_WARNING("Material \"{}\" constant buffer \"{}\" is at slot {} but expected slot {}.", m_path.c_str(), MATERIAL_CONSTANT_BUFFER_NAME, cbuffer->slot, MATERIAL_CONSTANT_BUFFER_SLOT);
				}
			}
		}
	}
	catch (const json::parse_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to parse material file \"{}\".\nDetails:\n{}", m_path.c_str(), error.what());
	}
	catch (const json::out_of_range& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read material file \"{}\".\nDetails:\n{}", m_path.c_str(), error.what());
	}
	catch (const json::type_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read material file \"{}\".\nDetails:\n{}", m_path.c_str(), error.what());
	}
}

Material::~Material() {}

bool Material::hasProperties() const noexcept
{
	return m_properties.get() != nullptr;
}

const vector<Material::TextureBinding>& Material::getTextures() const noexcept
{
	return m_textures;
}

Shader& Material::getVertexShader() noexcept
{
	return *m_vertexShader;
}

Shader& Material::getPixelShader() noexcept
{
	return *m_pixelShader;
}

GraphicsPipelineState& Material::getGraphicsPipelineState() noexcept
{
	return *m_pipeline;
}

ConstantBuffer& Material::getProperties()
{
	if (!hasProperties()) {
		GENESIS_LOG_THROW_ERROR("Material \"{}\" has no properties buffer.", m_path.c_str());
	}
	return *m_properties;
}

/* STATIC FUNCTION DEFINITIONS */

static SharedPtr<Shader> getShader(json& data, ShaderType type, ResourceManager& resourceManager)
{
	string path{};
	string entry{};
	auto& shaders = data.at("shaders");

	switch (type) {
	case ShaderType::VertexShader: 
	{
		path = shaders.at("vertex").at("path");
		entry = shaders.at("vertex").at("entry");
	}
	break;
	case ShaderType::PixelShader:
	{
		path = shaders.at("pixel").at("path");
		entry = shaders.at("pixel").at("entry");
	} 
	break;
	}

	return resourceManager.getShader(path.c_str(), entry.c_str(), type);
}

static vector<uint8> getPropertiesValues(json& data, const ShaderReflectionConstantBuffer& cbuffer, const char* path, Logger& logger)
{
	vector<uint8> values(cbuffer.size, 0);

	for (auto& [name, value] : data["properties"].items()) {
		auto it = cbuffer.variables.find(name);
		if (it == cbuffer.variables.end()) {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Material \"{}\" property \"{}\" not found, skipping.", path, name);
			continue;
		}

		const ShaderReflectionVariable& var = it->second;

		if (value.is_number_float()) {
			float f = value.get<float>();
			memcpy(values.data() + var.offset, &f, var.size);
		}
		else if (value.is_array()) {
			vector<float> fs = value.get<vector<float>>();
			size_t size = fs.size() * sizeof(float);

			if (size > var.size) {
				GENESIS_LOG(logger, Logger::LogLevel::Warning, "Material \"{}\" property \"{}\" array is larger than expected, truncating.", path, name);
			}
			memcpy(values.data() + var.offset, fs.data(), min<size_t>(var.size, size));
		}
		else if (value.is_number_integer()) {
			int32 i = value.get<int32>();
			memcpy(values.data() + var.offset, &i, var.size);
		}
		else {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Material \"{}\" property \"{}\" has unsupported type, skipping.", path, name);
		}
	}

	return values;
}