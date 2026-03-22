#include <resources/Material.h>
#include <resources/Texture.h>
#include <resources/VertexShader.h>
#include <resources/PixelShader.h>
#include <resources/ResourceManager.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <nlohmann/json.hpp>
#include <cmath>

#define MATERIAL_CONSTANT_BUFFER_NAME "MaterialProperties"

using namespace genesis;
using namespace std;
using namespace nlohmann;

static SharedPtr<VertexShader> getVertexShader(json& data, ResourceManager& resourceManager);
static SharedPtr<PixelShader> getPixelShader(json& data, ResourceManager& resourceManager);
static vector<uint8> getPropertiesValues(json& data, const ShaderConstantBuffer& cbuffer, Logger& logger);

Material::Material(const MaterialDesc& desc): Resource(desc.resource)
{
	try {
		json data = json::parse(resourcesUtils::readFile(m_path.c_str()));

		auto m_vertexShader = getVertexShader(data, desc.resource.resourceManager);
		auto m_pixelShader = getPixelShader(data, desc.resource.resourceManager);
		m_pipeline = desc.resource.graphicsDevice.createGraphicsPipelineState({m_vertexShader->getSignature(), m_pixelShader->getSignature()});

		if (data.contains("textures")) {
			for (auto& texture : data["textures"]) {
				string path = texture.at("path");
				uint32 slot = texture.at("slot");
				m_textures.push_back(desc.resource.resourceManager.getTexture(path.c_str()));
			}
		}
		
		if (data.contains("properties")) {
			auto& psSignature = m_pixelShader->getSignature();
			const ShaderConstantBuffer* cbuffer = psSignature.getConstantBuffer(MATERIAL_CONSTANT_BUFFER_NAME);
			
			if (cbuffer) {
				vector<uint8> buffer = getPropertiesValues(data, *cbuffer, getLogger());
				m_properties = desc.resource.graphicsDevice.createConstantBuffer({buffer.data(), static_cast<uint32>(buffer.size())});
				m_propertiesSlot = cbuffer->slot;
			}
			else {
				GENESIS_LOG_WARNING("Material \"{}\" has properties defined but shader has no constant buffer \"{}\".", m_path.c_str(), MATERIAL_CONSTANT_BUFFER_NAME);
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

uint32 Material::getPropertiesSlot() const
{
	if (!hasProperties()) {
		GENESIS_LOG_THROW_ERROR("Material \"{}\" has no properties buffer.", m_path.c_str());
	}
	return m_propertiesSlot;
}

const vector<SharedPtr<Texture>>& Material::getTextures() const noexcept
{
	return m_textures;
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

static SharedPtr<VertexShader> getVertexShader(json& data, ResourceManager& resourceManager)
{
	string path = data.at("shaders").at("vertex").at("path");
	string entry = data.at("shaders").at("vertex").at("entry");
	return resourceManager.getVertexShader(path.c_str(), entry.c_str());
}

static SharedPtr<PixelShader> getPixelShader(json& data, ResourceManager& resourceManager)
{
	string path = data.at("shaders").at("pixel").at("path");
	string entry = data.at("shaders").at("pixel").at("entry");
	return resourceManager.getPixelShader(path.c_str(), entry.c_str());
}

static vector<uint8> getPropertiesValues(json& data, const ShaderConstantBuffer& cbuffer, Logger& logger)
{
	vector<uint8> values(cbuffer.size, 0);

	for (auto& [name, value] : data["properties"].items()) {
		auto it = cbuffer.variables.find(name);
		if (it == cbuffer.variables.end()) {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Property \"{}\" not found in material properties, skipping.", name);
			continue;
		}

		const ShaderVariable& var = it->second;

		if (value.is_number_float()) {
			float f = value.get<float>();
			memcpy(values.data() + var.offset, &f, var.size);
		}
		else if (value.is_array()) {
			vector<float> fs = value.get<vector<float>>();
			size_t size = fs.size() * sizeof(float);
			memcpy(values.data() + var.offset, fs.data(), min<size_t>(var.size, size));
		}/*
		else if (value.is_number_integer()) {
			int32 i = value.get<int32>();
			memcpy(values.data() + var.offset, &i, var.size);
		}*/
		else {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Property \"{}\" has unsupported type, skipping.", name);
		}
	}

	return values;
}