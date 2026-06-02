#ifndef GENESIS_RESOURCES_UTILS_H
#define GENESIS_RESOURCES_UTILS_H
#include <core/Core.h>
#include <fstream>

GENESIS_NAMESPACE_BEGIN

namespace resourcesUtils
{
	inline std::string readFile(const char* filepath)
	{
		std::ifstream stream(filepath);
		if (!stream) {
			return std::string{};
		}
		return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	}

	inline void generateFont(const char* font, uint32 size, const char* outputFile, Logger& logger)
	{
		std::string command = "makespritefont.exe \"";
		command += font;
		command += "\" \"";
		command += outputFile;
		command += "\" /FontSize:";
		command += std::to_string(size);

		int result = std::system(command.c_str());
		if (result != 0) {
			GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Failed to generate font '{}' at size {}.", font, size);
		}
	}

	inline const char* textureFilterToString(SamplerFilter filter)
	{
		switch (filter) {
		case SamplerFilter::Point:
			return "point";
		case SamplerFilter::Bilinear:
			return "bilinear";
		case SamplerFilter::Trilinear:
			return "trilinear";
		case SamplerFilter::Anisotropic:
			return "anisotropic";
		default:
			return "unknown";
		}
	}

	inline const char* textureAddressModeToString(SamplerAddressMode mode)
	{
		switch (mode) {
		case SamplerAddressMode::Wrap:
			return "wrap";
		case SamplerAddressMode::Clamp:
			return "clamp";
		case SamplerAddressMode::Mirror:
			return "mirror";
		default:
			return "unknown";
		}
	}

	inline SamplerFilter stringToTextureFilter(const String& filter, const Logger& logger, const char* errorPrefix)
	{
		if (filter == "point") {
			return SamplerFilter::Point;
		}
		if (filter == "bilinear") {
			return SamplerFilter::Bilinear;
		}
		if (filter == "trilinear") {
			return SamplerFilter::Trilinear;
		}
		if (filter == "anisotropic") {
			return SamplerFilter::Anisotropic;
		}
		GENESIS_LOG(logger, Logger::LogLevel::Warning, "{} Unknown sampler filter: \"{}\". Using default \"{}\".", errorPrefix, filter.c_str(), textureFilterToString(SamplerFilter::Trilinear));
		return SamplerFilter::Trilinear;
	}

	inline SamplerAddressMode stringToTextureAddressMode(const String& mode, const Logger& logger, const char* errorPrefix)
	{
		if (mode == "wrap") {
			return SamplerAddressMode::Wrap;
		}
		if (mode == "clamp") {
			return SamplerAddressMode::Clamp;
		}
		if (mode == "mirror") {
			return SamplerAddressMode::Mirror;
		}
		GENESIS_LOG(logger, Logger::LogLevel::Warning, "{} Unknown texture address mode \"{}\". Using default \"{}\".", errorPrefix, mode.c_str(), textureAddressModeToString(SamplerAddressMode::Wrap));
		return SamplerAddressMode::Wrap;
	}
}

GENESIS_NAMESPACE_END

#endif