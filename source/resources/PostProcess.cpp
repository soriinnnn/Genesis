#include <resources/PostProcess.h>
#include <resources/Shader.h>
#include <resources/ResourceManager.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <graphics/EngineShaders.h>
#include <graphics/utils/GraphicsMacros.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Vec4.h>
#include <nlohmann/json.hpp>

using namespace genesis;
using namespace std;
using Json = nlohmann::json;
using LogLevel = Logger::LogLevel;

#define POST_EFFECT_ERROR_PREFIX "Failed to read post processing effect file \"{}\".\nDetails:\n"
#define REQUIRE_KEY(json, key, logger, path)\
{\
	if (!json.contains(key)) {\
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, POST_EFFECT_ERROR_PREFIX "Missing \"{}\" field.", path, key);\
	}\
}

#define KEY_RESOURCE_PATH "path"
#define KEY_RESOURCE_SLOT "slot"

#define KEY_POST_EFFECT_SHADER "shader"
#define KEY_POST_EFFECT_PROPERTIES "properties"
#define KEY_SHADER_ENTRY "entry"

#define DEFAULT_SHADER_ENTRY "main"

static SharedPtr<Shader> getShaderFromJSON(const Json& data, ShaderType type, ResourceManager& resourceManager, const Logger& logger, const char* effectPath);
static SharedPtr<ConstantBuffer> getConstantBufferFromJSON(const Json& data, const SharedPtr<Shader>& ps, GraphicsDevice& graphicsDevice, Vector<uint8>& buffer, HashMap<String, ShaderReflectionVariable>& variables, const Logger& logger, const char* effectPath);
static Vector<uint8> getPropertiesFromJSON(const Json& data, const ShaderReflectionConstantBuffer& reflection, const Logger& logger, const char* effectPath);

PostProcess::PostProcess(const PostProcessDesc& desc): Resource(desc.resource), m_isDirty{false}
{
	auto& context = desc.resource.graphicsContext;

	try {
		Json data = Json::parse(resourcesUtils::readFile(getPath()));

		SharedPtr<Shader> ps = getShaderFromJSON(data, ShaderType::PixelShader, desc.resource.resourceManager, getLogger(), getPath());
		m_pipeline = context.graphicsDevice.createGraphicsPipelineState({
			*context.engineShaders.getFullscreenTriangle().vsBinary, ps->getBinary(),
			*context.engineShaders.getFullscreenTriangle().vsSignature, ps->getSignature(),
			PrimitiveTopology::Triangles,
			false
		});
		
		m_properties = getConstantBufferFromJSON(data, ps, context.graphicsDevice, m_data, m_variables, getLogger(), getPath());
	}
	catch (const Json::parse_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to parse post processing effect file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
	catch (const Json::out_of_range& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read post processing effect file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
	catch (const Json::type_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read post processing effect file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
}

PostProcess::~PostProcess() {}

bool PostProcess::isDirty() const noexcept
{
	return m_isDirty;
}

bool PostProcess::hasProperties() const noexcept
{
	return m_properties != nullptr;
}

const GraphicsPipelineState& PostProcess::getGraphicsPipelineState() const noexcept
{
	return *m_pipeline;
}

const ConstantBuffer& PostProcess::getProperties() const
{
	if (!hasProperties()) {
		GENESIS_LOG_THROW_ERROR("Post processing effect \"{}\" has no properties buffer.", getPath());
	}
	return *m_properties;
}

BinaryData PostProcess::getData() const noexcept
{
	return BinaryData{m_data.data(), m_data.size()};
}

void PostProcess::clearDirty() noexcept
{
	m_isDirty = false;
}

void PostProcess::setProperty(const char* name, int value)
{
	setPropertyT<int>(name, value);
}

void PostProcess::setProperty(const char* name, float value)
{
	setPropertyT<float>(name, value);
}

void PostProcess::setProperty(const char* name, const Vec2& value)
{
	setPropertyT<Vec2>(name, value);
}

void PostProcess::setProperty(const char* name, const Vec3& value)
{
	setPropertyT<Vec3>(name, value);
}

void PostProcess::setProperty(const char* name, const Vec4& value)
{
	setPropertyT<Vec4>(name, value);
}

/* STATIC FUNCTION DEFINITIONS */

SharedPtr<Shader> getShaderFromJSON(const Json& data, ShaderType type, ResourceManager& resourceManager, const Logger& logger, const char* effectPath)
{
	REQUIRE_KEY(data, KEY_POST_EFFECT_SHADER, logger, effectPath);
	const Json& shader = data[KEY_POST_EFFECT_SHADER];
	
	REQUIRE_KEY(shader, KEY_RESOURCE_PATH, logger, effectPath);
	String shaderPath = shader[KEY_RESOURCE_PATH];

	String shaderEntry{};
	if (shader.contains(KEY_SHADER_ENTRY)) {
		shaderEntry = shader[KEY_SHADER_ENTRY];
	}
	else {
		GENESIS_LOG(logger, LogLevel::Warning, "Missing entry point in post processing effect \"{}\" for shader \"{}\". Using default \"{}\".", effectPath, shaderPath, DEFAULT_SHADER_ENTRY);
		shaderEntry = DEFAULT_SHADER_ENTRY;
	}

	return resourceManager.getShader(shaderPath.c_str(), shaderEntry.c_str(), type);
}

SharedPtr<ConstantBuffer> getConstantBufferFromJSON(const Json& data, const SharedPtr<Shader>& ps, GraphicsDevice& graphicsDevice, Vector<uint8>& buffer, HashMap<String, ShaderReflectionVariable>& variables, const Logger& logger, const char* effectPath)
{
	if (!data.contains(KEY_POST_EFFECT_PROPERTIES)) {
		GENESIS_LOG(logger, LogLevel::Warning, "No properties defined in post processing effect \"{}\".", effectPath);
		return nullptr;
	}

	const ShaderReflectionConstantBuffer* reflection = nullptr;
	const ShaderSignature& psSignature = ps->getSignature();

	if (psSignature.hasConstantBuffer(POST_PROCESSING_EFFECT_CONSTANT_BUFFER_NAME)) {
		reflection = psSignature.getConstantBufferReflection(POST_PROCESSING_EFFECT_CONSTANT_BUFFER_NAME);
	}
	else {
		GENESIS_LOG(logger, LogLevel::Warning, "Missing constant buffer \"{}\" definition in post processing effect \"{}\". Ignoring properties.", POST_PROCESSING_EFFECT_CONSTANT_BUFFER_NAME, effectPath);
		return nullptr;
	}

	if (reflection->slot != POST_PROCESSING_EFFECT_CONSTANT_BUFFER_SLOT) {
		GENESIS_LOG_THROW(logger, std::runtime_error, LogLevel::Error, POST_EFFECT_ERROR_PREFIX "Constant buffer \"{}\" is assigned to slot {} but expected slot {}.", effectPath, POST_PROCESSING_EFFECT_CONSTANT_BUFFER_NAME, reflection->slot, POST_PROCESSING_EFFECT_CONSTANT_BUFFER_SLOT);
	}

	variables = reflection->variables;
	buffer = getPropertiesFromJSON(data, *reflection, logger, effectPath);
	if (buffer.empty()) {
		return nullptr;
	}

	return graphicsDevice.createConstantBuffer({buffer.data(), static_cast<uint32>(buffer.size())});
}

Vector<uint8> getPropertiesFromJSON(const Json& data, const ShaderReflectionConstantBuffer& reflection, const Logger& logger, const char* effectPath)
{
	if (!data.contains(KEY_POST_EFFECT_PROPERTIES)) {
		return {};
	}

	Vector<uint8> values(reflection.size, 0);

	for (auto& [name, value] : data[KEY_POST_EFFECT_PROPERTIES].items()) {
		auto it = reflection.variables.find(name);
		if (it == reflection.variables.end()) {
			GENESIS_LOG(logger, LogLevel::Warning, "Missing property \"{}\" in post processing file \"{}\". Skipping.", name, effectPath);
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
				GENESIS_LOG(logger, Logger::LogLevel::Warning, "Property \"{}\" array is larger than expected in post processing file \"{}\". Truncating.", name, effectPath);
			}
			memcpy(values.data() + variable.offset, array.data(), min<size_t>(variable.size, size));
		}
		else {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Property \"{}\" has unsupported type in post processing file \"{}\". Skipping.", name, effectPath);
		}
	}

	return values;
}