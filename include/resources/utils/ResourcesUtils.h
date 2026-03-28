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
	}
}

#endif