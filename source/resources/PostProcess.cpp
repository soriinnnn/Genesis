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
using namespace nlohmann;

static SharedPtr<Shader> getShader(json& data, ShaderType type, ResourceManager& resourceManager, const char* path, Logger& logger);
static Vector<uint8> getPropertiesValues(json& data, const ShaderReflectionConstantBuffer& cbuffer, const char* path, Logger& logger);

PostProcess::PostProcess(const PostProcessDesc& desc): Resource(desc.resource), m_isDirty{false}
{
	auto& graphicsContext = desc.resource.graphicsContext;

	try {
		json data = json::parse(resourcesUtils::readFile(getPath()));

		SharedPtr<Shader> pixelShader = getShader(data, ShaderType::PixelShader, desc.resource.resourceManager, getPath(), getLogger());
		m_pipeline = graphicsContext.graphicsDevice.createGraphicsPipelineState({
			*graphicsContext.engineShaders.getFullscreenTriangle().vsBinary,
			pixelShader->getBinary(),
			*graphicsContext.engineShaders.getFullscreenTriangle().vsSignature,
			pixelShader->getSignature(),
			PrimitiveTopology::Triangles,
			false
		});
		
		const ShaderReflectionConstantBuffer* cbuffer = nullptr;
		if (pixelShader->getSignature().hasConstantBuffer(POST_PROCESSING_EFFECT_CONSTANT_BUFFER_NAME)) {
			cbuffer = pixelShader->getSignature().getConstantBufferReflection(POST_PROCESSING_EFFECT_CONSTANT_BUFFER_NAME);
		}

		if (cbuffer && data.contains("properties")) {
			m_data = getPropertiesValues(data, *cbuffer, getPath(), getLogger());
			m_variables = cbuffer->variables;
			if (cbuffer->slot != POST_PROCESSING_EFFECT_CONSTANT_BUFFER_SLOT) {
				GENESIS_LOG_WARNING("Post processing effect \"{}\" constant buffer \"{}\" is at slot {} but expected slot {}.", getPath(), POST_PROCESSING_EFFECT_CONSTANT_BUFFER_NAME, cbuffer->slot, POST_PROCESSING_EFFECT_CONSTANT_BUFFER_SLOT);
			}
		}
		else if (!cbuffer && data.contains("properties")) {
			GENESIS_LOG_WARNING("Post processing effect \"{}\" has properties defined but no \"{}\" constant buffer found in shader, skipping.", getPath(), POST_PROCESSING_EFFECT_CONSTANT_BUFFER_NAME);
		}

		if (!m_data.empty()) {
			m_properties = graphicsContext.graphicsDevice.createConstantBuffer({m_data.data(), static_cast<uint32>(m_data.size())});
		}
	}
	catch (const json::parse_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to parse post processing effect file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
	catch (const json::out_of_range& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read post processing effect file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
	catch (const json::type_error& error) {
		GENESIS_LOG_THROW_ERROR("Failed to read post processing effect file \"{}\".\nDetails:\n{}", getPath(), error.what());
	}
}

PostProcess::~PostProcess() {}

bool PostProcess::hasProperties() const noexcept
{
	return m_properties != nullptr;
}

bool PostProcess::isDirty() const noexcept
{
	return m_isDirty;
}

void PostProcess::clearDirty() noexcept
{
	m_isDirty = false;
}

GraphicsPipelineState& PostProcess::getGraphicsPipelineState() noexcept
{
	return *m_pipeline;
}

ConstantBuffer& PostProcess::getProperties()
{
	if (!hasProperties()) {
		GENESIS_LOG_THROW_ERROR("Post processing effect \"{}\" has no properties buffer.", getPath());
	}
	return *m_properties;
}

uint8* PostProcess::getData()
{
	return m_data.data();
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

SharedPtr<Shader> getShader(json& data, ShaderType type, ResourceManager& resourceManager, const char* path, Logger& logger)
{
	if (!data.contains("shader")) {
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Post processing effect file \"{}\" is missing \"shader\" field.", path);
	}
	if (!data["shader"].contains("path")) {
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Post processing effect file \"{}\" is missing \"path\" field.", path);
	}
	if (!data["shader"].contains("entry")) {
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Post processing effect file \"{}\" is missing \"entry\" field.", path);
	}

	string shaderPath = data["shader"]["path"];
	string shaderEntry = data["shader"]["entry"];
	return resourceManager.getShader(shaderPath.c_str(), shaderEntry.c_str(), type);
}

Vector<uint8> getPropertiesValues(json& data, const ShaderReflectionConstantBuffer& cbuffer, const char* path, Logger& logger)
{
	Vector<uint8> values(cbuffer.size);

	for (auto& [name, value] : data["properties"].items()) {
		auto it = cbuffer.variables.find(name);
		if (it == cbuffer.variables.end()) {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Post processing effect \"{}\" property \"{}\" not found, skipping.", path, name);
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
				GENESIS_LOG(logger, Logger::LogLevel::Warning, "Post processing effect \"{}\" property \"{}\" array is larger than expected, truncating.", path, name);
			}
			memcpy(values.data() + var.offset, fs.data(), min<size_t>(var.size, size));
		}
		else if (value.is_number_integer()) {
			int32 i = value.get<int32>();
			memcpy(values.data() + var.offset, &i, var.size);
		}
		else {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Post processing effect \"{}\" property \"{}\" has unsupported type, skipping.", path, name);
		}
	}

	return values;
}