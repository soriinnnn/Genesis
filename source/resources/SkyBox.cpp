#include <resources/SkyBox.h>
#include <resources/ResourceManager.h>
#include <resources/utils/ResourcesUtils.h>
#include <graphics/GraphicsDevice.h>
#include <nlohmann/json.hpp>

using namespace genesis;
using namespace std;
using namespace resourcesUtils;
using Json = nlohmann::json;
using LogLevel = Logger::LogLevel;

#define SKYBOX_ERROR_PREFIX "Failed to read skybox file \"{}\".\nDetails:\n"
#define REQUIRE_KEY(json, key, logger, path)\
{\
	if (!json.contains(key)) {\
		GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, SKYBOX_ERROR_PREFIX "Missing \"{}\" field.", path, key);\
	}\
}

#define KEY_SKYBOX_TEXTURES "textures"
#define KEY_SKYBOX_SAMPLER "sampler"

#define KEY_TEXTURES_FRONT_FACE "front"
#define KEY_TEXTURES_BACK_FACE "back"
#define KEY_TEXTURES_RIGHT_FACE "right"
#define KEY_TEXTURES_LEFT_FACE "left"
#define KEY_TEXTURES_TOP_FACE "top"
#define KEY_TEXTURES_BOTTOM_FACE "bottom"

#define KEY_SAMPLER_FILTER "filter"
#define KEY_SAMPLER_ADDRESS_UV "addressUV"
#define KEY_SAMPLER_ADDRESS_W "addressW"
#define KEY_SAMPLER_MAX_ANISOTROPY "maxAnisotropy"

#define DEFAULT_SAMPLER_FILTER SamplerFilter::Trilinear
#define DEFAULT_SAMPLER_ADDRESS_MODE SamplerAddressMode::Wrap
#define DEFAULT_SAMPLER_MAX_ANISOTROPY 16

static SharedPtr<CubeTexture> getTextureFromJSON(const Json& data, GraphicsDevice& graphicsDevice, const Logger& logger, const char* path);
static SharedPtr<SamplerState> getSamplerStateFromJSON(const Json& data, GraphicsDevice& graphicsDevice, const Logger& logger, const char* path);

SkyBox::SkyBox(const SkyBoxDesc& desc): Resource(desc.resource)
{
	auto context = desc.resource.graphicsContext;

	try {
		Json data = Json::parse(resourcesUtils::readFile(getPath()));

		m_texture = getTextureFromJSON(data, context.graphicsDevice, getLogger(), getPath());
		if (data.contains(KEY_SKYBOX_SAMPLER)) {
			m_sampler = getSamplerStateFromJSON(data[KEY_SKYBOX_SAMPLER], context.graphicsDevice, getLogger(), getPath());
		}
		else {
			//GENESIS_LOG_WARNING("No sampler specified. Using default sampler for skybox \"{}\".", getPath());
			m_sampler = context.graphicsDevice.createSamplerState({SamplerFilter::Trilinear, SamplerAddressMode::Clamp, SamplerAddressMode::Clamp, SamplerAddressMode::Clamp});
		}
	}
	catch (const Json::parse_error& error) {
		GENESIS_LOG_THROW_ERROR(SKYBOX_ERROR_PREFIX "{}", getPath(), error.what());
	}
	catch (const Json::out_of_range& error) {
		GENESIS_LOG_THROW_ERROR(SKYBOX_ERROR_PREFIX "{}", getPath(), error.what());
	}
	catch (const Json::type_error& error) {
		GENESIS_LOG_THROW_ERROR(SKYBOX_ERROR_PREFIX "{}", getPath(), error.what());
	}
}

SkyBox::~SkyBox() {}

const CubeTexture& SkyBox::getCubeTexture() const
{
	return *m_texture;
}

const SamplerState& SkyBox::getSamplerState() const
{
	return *m_sampler;
}

/* STATIC FUNCTION DEFINITIONS */

SharedPtr<CubeTexture> getTextureFromJSON(const Json& data, GraphicsDevice& graphicsDevice, const Logger& logger, const char* path)
{
	REQUIRE_KEY(data, KEY_SKYBOX_TEXTURES, logger, path);
	const Json& textures = data[KEY_SKYBOX_TEXTURES];

	const char* faceKeys[6] = {
			KEY_TEXTURES_RIGHT_FACE,
			KEY_TEXTURES_LEFT_FACE,
			KEY_TEXTURES_TOP_FACE,
			KEY_TEXTURES_BOTTOM_FACE,
			KEY_TEXTURES_FRONT_FACE,
			KEY_TEXTURES_BACK_FACE
	};

	String faces[6];
	CubeTextureDesc desc{};

	for (int i = 0; i < 6; i++) {
		REQUIRE_KEY(textures, faceKeys[i], logger, path);
		faces[i] = textures[faceKeys[i]];
		desc.cubeFaces[i] = faces[i].c_str();
	}

	return graphicsDevice.createCubeTexture(desc);
}

SharedPtr<SamplerState> getSamplerStateFromJSON(const Json& data, GraphicsDevice& graphicsDevice, const Logger& logger, const char* path)
{
	SamplerStateDesc desc{};

	if (data.contains(KEY_SAMPLER_FILTER)) {
		String filter = data[KEY_SAMPLER_FILTER];
		std::transform(filter.begin(), filter.end(), filter.begin(), ::tolower);
		desc.filter = stringToTextureFilter(filter, logger, format(SKYBOX_ERROR_PREFIX, path).c_str());
	}
	else {
		GENESIS_LOG(logger, LogLevel::Warning, "Missing sampler texture filter in skybox \"{}\". Using default \"{}\".", path, textureFilterToString(DEFAULT_SAMPLER_FILTER));
		desc.filter = DEFAULT_SAMPLER_FILTER;
	}

	if (data.contains(KEY_SAMPLER_ADDRESS_UV)) {
		String mode = data[KEY_SAMPLER_ADDRESS_UV];
		std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
		desc.addressU = stringToTextureAddressMode(mode, logger, format(SKYBOX_ERROR_PREFIX, path).c_str());
		desc.addressV = desc.addressU;
	}
	else {
		GENESIS_LOG(logger, LogLevel::Warning, "Missing sampler texture address mode in skybox \"{}\". Using default \"{}\".", path, textureAddressModeToString(DEFAULT_SAMPLER_ADDRESS_MODE));
		desc.addressU = DEFAULT_SAMPLER_ADDRESS_MODE;
		desc.addressV = DEFAULT_SAMPLER_ADDRESS_MODE;
	}

	if (data.contains(KEY_SAMPLER_ADDRESS_W)) {
		String mode = data[KEY_SAMPLER_ADDRESS_W];
		std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
		desc.addressW = stringToTextureAddressMode(mode, logger, format(SKYBOX_ERROR_PREFIX, path).c_str());
	}
	else {
		desc.addressW = DEFAULT_SAMPLER_ADDRESS_MODE;
	}

	if (data.contains(KEY_SAMPLER_MAX_ANISOTROPY)) {
		uint32 maxAnisotropy = data[KEY_SAMPLER_MAX_ANISOTROPY];
		desc.maxAnisotropy = maxAnisotropy;
	}
	else {
		if (desc.filter == SamplerFilter::Anisotropic) {
			GENESIS_LOG(logger, LogLevel::Warning, "Missing sampler max anisotropy in skybox \"{}\". Using default \"{}\".", path, DEFAULT_SAMPLER_MAX_ANISOTROPY);
			desc.maxAnisotropy = DEFAULT_SAMPLER_MAX_ANISOTROPY;
		}
	}

	return graphicsDevice.createSamplerState(desc);
}