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
using Json = nlohmann::json;
using LogLevel = Logger::LogLevel;

#define POST_EFFECT_ERROR_PREFIX "Failed to read material file \"{}\".\nDetails:\n"
#define REQUIRE_KEY(json, key, logger, path)\
{\
	if (!json.contains(key)) {\
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, POST_EFFECT_ERROR_PREFIX "Missing \"{}\" field.", path, key);\
	}\
}

#define KEY_RESOURCE_PATH "path"
#define KEY_RESOURCE_SLOT "slot"

#define KEY_MATERIAL_SHADERS "shaders"
#define KEY_MATERIAL_TEXTURES "textures"
#define KEY_MATERIAL_SAMPLERS "samplers"
#define KEY_MATERIAL_PROPERTIES "properties"

#define KEY_SHADER_TYPE_VERTEX "vertex"
#define KEY_SHADER_TYPE_PIXEL "pixel"
#define KEY_SHADER_ENTRY "entry"

#define KEY_SAMPLER_FILTER "filter"
#define KEY_SAMPLER_ADDRESS_UV "addressUV"
#define KEY_SAMPLER_ADDRESS_W "addressW"
#define KEY_SAMPLER_MAX_ANISOTROPY "maxAnisotropy"

#define DEFAULT_RESOURCE_SLOT 0
#define DEFAULT_SHADER_ENTRY "main"
#define DEFAULT_SAMPLER_FILTER TextureFilter::Anisotropic
#define DEFAULT_SAMPLER_ADDRESS_MODE TextureAddressMode::Wrap
#define DEFAULT_SAMPLER_MAX_ANISOTROPY 16

static SharedPtr<Shader> getShaderFromJSON(const Json& data, ShaderType type, ResourceManager& resourceManager, const Logger& logger, const char* materialPath);
static Vector<Material::TextureBinding> getTexturesFromJSON(const Json& data, ResourceManager& resourceManager, const Logger& logger, const char* materialPath);
static Vector<Material::SamplerBinding> getSamplersFromJSON(const Json& data, GraphicsDevice& graphicsDevice, const Logger& logger, const char* materialPath);
static SharedPtr<SamplerState> getSamplerStateFromJSON(const Json& data, GraphicsDevice& graphicsDevice, const Logger& logger, const char* materialPath);
static SharedPtr<ConstantBuffer> getConstantBufferFromJSON(const Json& data, const SharedPtr<Shader>& vs, const SharedPtr<Shader>& ps, GraphicsDevice& graphicsDevice, const Logger& logger, const char* materialPath);
static Vector<uint8> getPropertiesFromJSON(const Json& data, const ShaderReflectionConstantBuffer& reflection, const Logger& logger, const char* materialPath);

static TextureFilter stringToTextureFilter(const String& filter, const Logger& logger, const char* materialPath);
static TextureAddressMode stringToTextureAddressMode(const String& mode, const Logger& logger, const char* materialPath);
static const char* textureFilterToString(TextureFilter filter);
static const char* textureAddressModeToString(TextureAddressMode mode);

Material::Material(const MaterialDesc& desc): Resource(desc.resource)
{
	auto& context = desc.resource.graphicsContext;

	try {
		Json data = Json::parse(resourcesUtils::readFile(getPath()));

		SharedPtr<Shader> vs = getShaderFromJSON(data, ShaderType::VertexShader, desc.resource.resourceManager, getLogger(), getPath());
		SharedPtr<Shader> ps = getShaderFromJSON(data, ShaderType::PixelShader, desc.resource.resourceManager, getLogger(), getPath());
		m_pipeline = context.graphicsDevice.createGraphicsPipelineState({
			vs->getBinary(), 
			ps->getBinary(), 
			vs->getSignature(), 
			ps->getSignature()
		});

		m_textures = getTexturesFromJSON(data, desc.resource.resourceManager, getLogger(), getPath());
		m_samplers = getSamplersFromJSON(data, context.graphicsDevice, getLogger(), getPath());
		m_properties = getConstantBufferFromJSON(data, vs, ps, context.graphicsDevice, getLogger(), getPath());
	}
	catch (const Json::parse_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to parse material file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
	catch (const Json::out_of_range& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read material file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
	catch (const Json::type_error& error) {
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

static SharedPtr<Shader> getShaderFromJSON(const Json& data, ShaderType type, ResourceManager& resourceManager, const Logger& logger, const char* materialPath)
{
	REQUIRE_KEY(data, KEY_MATERIAL_SHADERS, logger, materialPath);
	const Json& shaders = data[KEY_MATERIAL_SHADERS];

	const char* shaderKey = nullptr;
	switch (type) {
		case ShaderType::VertexShader: {
			shaderKey = KEY_SHADER_TYPE_VERTEX;
			break;
		}
		case ShaderType::PixelShader: {
			shaderKey = KEY_SHADER_TYPE_PIXEL;
			break;
		default:
			GENESIS_ASSERT(false, "Unknown shader type.");
		}
	}

	REQUIRE_KEY(shaders, shaderKey, logger, materialPath);
	const Json& shader = shaders[shaderKey];

	REQUIRE_KEY(shader, KEY_RESOURCE_PATH, logger, materialPath);
	String shaderPath = shader[KEY_RESOURCE_PATH];

	String shaderEntry{};
	if (shader.contains(KEY_SHADER_ENTRY)) {
		shaderEntry = shader[KEY_SHADER_ENTRY];
	}
	else {
		GENESIS_LOG(logger, LogLevel::Warning, "Missing entry point in material \"{}\" for shader \"{}\". Using default \"{}\".", materialPath, shaderPath, DEFAULT_SHADER_ENTRY);
		shaderEntry = DEFAULT_SHADER_ENTRY;
	}

	return resourceManager.getShader(shaderPath.c_str(), shaderEntry.c_str(), type);
}

Vector<Material::TextureBinding> getTexturesFromJSON(const Json& data, ResourceManager& resourceManager, const Logger& logger, const char* materialPath)
{
	if (!data.contains(KEY_MATERIAL_TEXTURES)) {
		return {};
	}

	Vector<Material::TextureBinding> textures;

	for (const Json& texture : data[KEY_MATERIAL_TEXTURES]) {
		REQUIRE_KEY(texture, KEY_RESOURCE_PATH, logger, materialPath);
		REQUIRE_KEY(texture, KEY_RESOURCE_SLOT, logger, materialPath);

		String path = texture[KEY_RESOURCE_PATH];
		uint32 slot = texture[KEY_RESOURCE_SLOT];
		
		textures.push_back({resourceManager.getTexture(path.c_str()), slot});
	}

	int size = static_cast<int>(textures.size());
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (textures[i].slot == textures[j].slot) {
				GENESIS_LOG_THROW(logger, std::runtime_error, LogLevel::Error, POST_EFFECT_ERROR_PREFIX "Duplicate texture slot.", materialPath);
			}
		}
	}

	return textures;
}

Vector<Material::SamplerBinding> getSamplersFromJSON(const Json& data, GraphicsDevice& graphicsDevice, const Logger& logger, const char* materialPath)
{
	if (!data.contains(KEY_MATERIAL_SAMPLERS)) {
		return {{graphicsDevice.createSamplerState({}), DEFAULT_RESOURCE_SLOT}};
	}

	Vector<Material::SamplerBinding> samplers;

	for (const Json& sampler : data[KEY_MATERIAL_SAMPLERS]) {
		REQUIRE_KEY(sampler, KEY_RESOURCE_SLOT, logger, materialPath);
		uint32 slot = sampler[KEY_RESOURCE_SLOT];
		samplers.push_back({getSamplerStateFromJSON(sampler, graphicsDevice, logger, materialPath), slot});
	}

	int size = static_cast<int>(samplers.size());
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			if (samplers[i].slot == samplers[j].slot) {
				GENESIS_LOG_THROW(logger, std::runtime_error, LogLevel::Error, POST_EFFECT_ERROR_PREFIX "Duplicate sampler slot.", materialPath);
			}
		}
	}

	return samplers;
}

SharedPtr<SamplerState> getSamplerStateFromJSON(const Json& data, GraphicsDevice& graphicsDevice, const Logger& logger, const char* materialPath)
{
	SamplerStateDesc desc{};

	if (data.contains(KEY_SAMPLER_FILTER)) {
		String filter = data[KEY_SAMPLER_FILTER];
		std::transform(filter.begin(), filter.end(), filter.begin(), ::tolower);
		desc.filter = stringToTextureFilter(filter, logger, materialPath);
	}
	else {
		GENESIS_LOG(logger, LogLevel::Warning, "Missing sampler texture filter in material \"{}\". Using default \"{}\".", materialPath, textureFilterToString(DEFAULT_SAMPLER_FILTER));
		desc.filter = DEFAULT_SAMPLER_FILTER;
	}

	if (data.contains(KEY_SAMPLER_ADDRESS_UV)) {
		String mode = data[KEY_SAMPLER_ADDRESS_UV];
		std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
		desc.addressU = stringToTextureAddressMode(mode, logger, materialPath);
		desc.addressV = desc.addressU;
	}
	else {
		GENESIS_LOG(logger, LogLevel::Warning, "Missing sampler texture address mode in material \"{}\". Using default \"{}\".", materialPath, textureAddressModeToString(DEFAULT_SAMPLER_ADDRESS_MODE));
		desc.addressU = DEFAULT_SAMPLER_ADDRESS_MODE;
		desc.addressV = DEFAULT_SAMPLER_ADDRESS_MODE;
	}

	if (data.contains(KEY_SAMPLER_ADDRESS_W)) {
		String mode = data[KEY_SAMPLER_ADDRESS_W];
		std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
		desc.addressW = stringToTextureAddressMode(mode, logger, materialPath);
	}
	else {
		desc.addressW = DEFAULT_SAMPLER_ADDRESS_MODE;
	}

	if (data.contains(KEY_SAMPLER_MAX_ANISOTROPY)) {
		uint32 maxAnisotropy = data[KEY_SAMPLER_MAX_ANISOTROPY];
		desc.maxAnisotropy = maxAnisotropy;
	}
	else {
		if (desc.filter == TextureFilter::Anisotropic) {
			GENESIS_LOG(logger, LogLevel::Warning, "Missing sampler max anisotropy in material \"{}\". Using default \"{}\".", materialPath, DEFAULT_SAMPLER_MAX_ANISOTROPY);
			desc.maxAnisotropy = DEFAULT_SAMPLER_MAX_ANISOTROPY;
		}
	}

	return graphicsDevice.createSamplerState(desc);
}

SharedPtr<ConstantBuffer> getConstantBufferFromJSON(const Json& data, const SharedPtr<Shader>& vs, const SharedPtr<Shader>& ps, GraphicsDevice& graphicsDevice, const Logger& logger, const char* materialPath)
{
	if (!data.contains(KEY_MATERIAL_PROPERTIES)) {
		GENESIS_LOG(logger, LogLevel::Warning, "No properties defined in material \"{}\".", materialPath);
		return nullptr;
	}

	const ShaderReflectionConstantBuffer* reflection = nullptr;
	const ShaderSignature& vsSignature = vs->getSignature();
	const ShaderSignature& psSignature = ps->getSignature();

	if (vsSignature.hasConstantBuffer(MATERIAL_CONSTANT_BUFFER_NAME)) {
		reflection = vsSignature.getConstantBufferReflection(MATERIAL_CONSTANT_BUFFER_NAME);
	}
	else if (psSignature.hasConstantBuffer(MATERIAL_CONSTANT_BUFFER_NAME)) {
		reflection = psSignature.getConstantBufferReflection(MATERIAL_CONSTANT_BUFFER_NAME);
	}
	else {
		GENESIS_LOG(logger, LogLevel::Warning, "Missing constant buffer \"{}\" definition in material \"{}\". Ignoring properties.", MATERIAL_CONSTANT_BUFFER_NAME, materialPath);
		return nullptr;
	}

	if (reflection->slot != MATERIAL_CONSTANT_BUFFER_SLOT) {
		GENESIS_LOG_THROW(logger, std::runtime_error, LogLevel::Error, POST_EFFECT_ERROR_PREFIX "Constant buffer \"{}\" is assigned to slot {} but expected slot {}.", materialPath, MATERIAL_CONSTANT_BUFFER_NAME, reflection->slot, MATERIAL_CONSTANT_BUFFER_SLOT);
	}

	Vector<uint8> buffer = getPropertiesFromJSON(data, *reflection, logger, materialPath);
	return graphicsDevice.createConstantBuffer({buffer.data(), static_cast<uint32>(buffer.size())});
}

Vector<uint8> getPropertiesFromJSON(const Json& data, const ShaderReflectionConstantBuffer& reflection, const Logger& logger, const char* materialPath)
{
	if (!data.contains(KEY_MATERIAL_PROPERTIES)) {
		return {};
	}

	Vector<uint8> values(reflection.size, 0);

	for (auto& [name, value] : data[KEY_MATERIAL_PROPERTIES].items()) {
		auto it = reflection.variables.find(name);
		if (it == reflection.variables.end()) {
			GENESIS_LOG(logger, LogLevel::Warning, "Missing property \"{}\" in material \"{}\". Skipping.", name, materialPath);
			continue;
		}

		const ShaderReflectionVariable& variable = it->second;

		if (value.is_number_integer()) {
			int32 integer = value.get<int32>();
			memcpy(values.data() + variable.offset, &integer, variable.size);
		}
		else if (value.is_number_float()) {
			float number = value.get<float>();
			memcpy(values.data() + variable.offset, &number, variable.size);
		}
		else if (value.is_array()) {
			Vector<float> array = value.get<Vector<float>>();
			size_t size = array.size() * sizeof(float);
			if (size > variable.size) {
				GENESIS_LOG(logger, Logger::LogLevel::Warning, "Property \"{}\" array is larger than expected in material \"{}\". Truncating.", name, materialPath);
			}
			memcpy(values.data() + variable.offset, array.data(), min<size_t>(variable.size, size));
		}
		else {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Property \"{}\" has unsupported type in material \"{}\". Skipping.", name, materialPath);
		}
	}

	return values;
}

TextureFilter stringToTextureFilter(const String& filter, const Logger& logger, const char* materialPath)
{
	if (filter == "point") {
		return TextureFilter::Point;
	}
	if (filter == "linear") {
		return TextureFilter::Linear;
	}
	if (filter == "anisotropic") {
		return TextureFilter::Anisotropic;
	}
	GENESIS_LOG_THROW(logger, std::runtime_error, LogLevel::Error, POST_EFFECT_ERROR_PREFIX "Unknown texture filter \"{}\".", materialPath, filter.c_str());
}

TextureAddressMode stringToTextureAddressMode(const String& mode, const Logger& logger, const char* materialPath)
{
	if (mode == "wrap") {
		return TextureAddressMode::Wrap;
	}
	if (mode == "clamp") {
		return TextureAddressMode::Clamp;
	}
	if (mode == "mirror") {
		return TextureAddressMode::Mirror;
	}
	GENESIS_LOG_THROW(logger, std::runtime_error, LogLevel::Error, POST_EFFECT_ERROR_PREFIX "Unknown texture address mode \"{}\".", materialPath, mode.c_str());
}

const char* textureFilterToString(TextureFilter filter)
{
	switch (filter) {
	case TextureFilter::Point:
		return "point";
	case TextureFilter::Linear:
		return "linear";
	case TextureFilter::Anisotropic:
		return "anisotropic";
	default:
		return "unknown";
	}
}

const char* textureAddressModeToString(TextureAddressMode mode)
{
	switch (mode) {
	case TextureAddressMode::Wrap:
		return "wrap";
	case TextureAddressMode::Clamp:
		return "clamp";
	case TextureAddressMode::Mirror:
		return "mirror";
	default:
		return "unknown";
	}
}