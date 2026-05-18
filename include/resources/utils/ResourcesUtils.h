#ifndef GENESIS_RESOURCES_UTILS_H
#define GENESIS_RESOURCES_UTILS_H
#include <fstream>

namespace genesis
{
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
	}
}

#endif