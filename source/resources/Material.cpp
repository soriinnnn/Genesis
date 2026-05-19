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

static SharedPtr<Shader> getShader(json& data, ShaderType type, ResourceManager& resourceManager, const char* path, Logger& logger);
static SharedPtr<SamplerState> getSamplerState(json& data, GraphicsDevice& graphicsDevice, const char* path, Logger& logger);
static Vector<uint8> getPropertiesValues(json& data, const ShaderReflectionConstantBuffer& cbuffer, const char* path, Logger& logger);

Material::Material(const MaterialDesc& desc): Resource(desc.resource)
{
	auto& graphicsContext = desc.resource.graphicsContext;

	try {
		json data = json::parse(resourcesUtils::readFile(getPath()));

		auto vertexShader = getShader(data, ShaderType::VertexShader, desc.resource.resourceManager, getPath(), getLogger());
		auto pixelShader = getShader(data, ShaderType::PixelShader, desc.resource.resourceManager, getPath(), getLogger());
		
		m_pipeline = graphicsContext.graphicsDevice.createGraphicsPipelineState({
			vertexShader->getBinary(), 
			pixelShader->getBinary(), 
			vertexShader->getSignature(), 
			pixelShader->getSignature()
		});

		if (data.contains("textures")) {
			for (auto& texture : data["textures"]) {
				if (!texture.contains("path")) {
					GENESIS_LOG_THROW_ERROR("Material \"{}\" texture is missing \"path\" field.", getPath());
				}
				if (!texture.contains("slot")) {
					GENESIS_LOG_THROW_ERROR("Material \"{}\" texture is missing \"slot\" field.", getPath());
				}

				string path = texture["path"];
				uint32 slot = texture["slot"];
				m_textures.push_back({desc.resource.resourceManager.getTexture(path.c_str()), slot});
			}
		}

		if (data.contains("samplers")) {
			for (auto& sampler : data["samplers"]) {
				if (!sampler.contains("slot")) {
					GENESIS_LOG_THROW_ERROR("Material \"{}\" texture sampler is missing \"slot\" field.", getPath());
				}

				uint32 slot = sampler["slot"];
				m_samplers.push_back({getSamplerState(sampler, graphicsContext.graphicsDevice, getPath(), getLogger()), slot});
			}
		}
		else {
			m_samplers.push_back({graphicsContext.graphicsDevice.createSamplerState({}), 0});
		}

		const ShaderReflectionConstantBuffer* cbuffer = nullptr;
		auto& vsSignature = vertexShader->getSignature();
		auto& psSignature = pixelShader->getSignature();

		if (vsSignature.hasConstantBuffer(MATERIAL_CONSTANT_BUFFER_NAME)) {
			cbuffer = vsSignature.getConstantBufferReflection(MATERIAL_CONSTANT_BUFFER_NAME);
		}
		else if (psSignature.hasConstantBuffer(MATERIAL_CONSTANT_BUFFER_NAME)) {
			cbuffer = psSignature.getConstantBufferReflection(MATERIAL_CONSTANT_BUFFER_NAME);
		}

		if (cbuffer && data.contains("properties")) {
			Vector<uint8> buffer = getPropertiesValues(data, *cbuffer, getPath(), getLogger());
			m_properties = graphicsContext.graphicsDevice.createConstantBuffer({buffer.data(), static_cast<uint32>(buffer.size())});
			if (cbuffer->slot != MATERIAL_CONSTANT_BUFFER_SLOT) {
				GENESIS_LOG_WARNING("Material \"{}\" constant buffer \"{}\" is at slot {} but expected slot {}.", getPath(), MATERIAL_CONSTANT_BUFFER_NAME, cbuffer->slot, MATERIAL_CONSTANT_BUFFER_SLOT);
			}
		}
		else if (!cbuffer && data.contains("properties")) {
			GENESIS_LOG_WARNING("Material \"{}\" has properties defined but no \"{}\" constant buffer found in shaders, skipping.", getPath(), MATERIAL_CONSTANT_BUFFER_NAME);
		}
	}
	catch (const json::parse_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to parse material file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
	catch (const json::out_of_range& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read material file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
	catch (const json::type_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read material file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
}

Material::~Material() {}

bool Material::hasProperties() const noexcept
{
	return m_properties.get() != nullptr;
}

const Vector<Material::TextureBinding>& Material::getTextures() const noexcept
{
	return m_textures;
}

const Vector<Material::SamplerBinding>& Material::getSamplers() const noexcept
{
	return m_samplers;
}

const GraphicsPipelineState& Material::getGraphicsPipelineState() const noexcept
{
	return *m_pipeline;
}

const ConstantBuffer& Material::getProperties() const
{
	if (!hasProperties()) {
		GENESIS_LOG_THROW_ERROR("Material \"{}\" has no properties buffer.", m_path.c_str());
	}
	return *m_properties;
}

/* STATIC FUNCTION DEFINITIONS */

static SharedPtr<Shader> getShader(json& data, ShaderType type, ResourceManager& resourceManager, const char* path, Logger& logger)
{
	if (!data.contains("shaders")) {
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Material \"{}\" is missing \"shaders\" field.", path);
	}

	json shader;
	switch (type) {
	case ShaderType::VertexShader:
		if (!data["shaders"].contains("vertex")) {
			GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Material \"{}\" is missing \"vertex\" shader.", path);
		}
		shader = data["shaders"]["vertex"];
		break;
	case ShaderType::PixelShader:
		if (!data["shaders"].contains("pixel")) {
			GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Material \"{}\" is missing \"pixel\" shader.", path);
		}
		shader = data["shaders"]["pixel"];
		break;
	}
	
	if (!shader.contains("path")) {
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Material \"{}\" shader is missing \"path\" field.", path);
	}
	if (!shader.contains("entry")) {
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Material \"{}\" shader is missing \"entry\" field.", path);
	}

	string shaderPath = shader["path"];
	string shaderEntry = shader["entry"];
	return resourceManager.getShader(shaderPath.c_str(), shaderEntry.c_str(), type);
}

SharedPtr<SamplerState> getSamplerState(json& data, GraphicsDevice& graphicsDevice, const char* path, Logger& logger)
{
	SamplerStateDesc desc{};

	if (data.contains("filter")) {
		string filter = data["filter"];

		if (filter == "Point") {
			desc.filter = TextureFilter::Point;
		}
		else if (filter == "Linear") {
			desc.filter = TextureFilter::Linear;
		}
		else if (filter == "Anisotropic") {
			desc.filter = TextureFilter::Anisotropic;
		}
		else {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Material \"{}\" has unknown sampler filter \"{}\", using default.", path, filter);
		}
	}

	auto getAddressMode = [&](const string& key) {
		if (!data.contains(key)) {
			return TextureAddressMode::Wrap;
		}

		string mode = data[key];
		if (mode == "Wrap") {
			return TextureAddressMode::Wrap;
		}
		if (mode == "Mirror") {
			return TextureAddressMode::Mirror;
		}
		else if (mode == "Clamp") {
			return TextureAddressMode::Clamp;
		}
		else {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Material \"{}\" has unknown sampler address mode \"{}\" for \"{}\", using Wrap.", path, mode, key);
		}
		return TextureAddressMode::Wrap;
	};

	desc.addressU = getAddressMode("addressU");
	desc.addressV = getAddressMode("addressV");
	desc.addressW = getAddressMode("addressW");

	if (data.contains("maxAnisotropy")) {
		desc.maxAnisotropy = data["maxAnisotropy"];
	}

	return graphicsDevice.createSamplerState(desc);
}

Vector<uint8> getPropertiesValues(json& data, const ShaderReflectionConstantBuffer& cbuffer, const char* path, Logger& logger)
{
	Vector<uint8> values(cbuffer.size, 0);

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