#ifndef GENESIS_LOG_UTILS_H
#define GENESIS_LOG_UTILS_H
#include <stdexcept>

#define GENESIS_LOG(logger, type, msg, ...)\
logger.log((type), msg __VA_OPT__(,) __VA_ARGS__)
	
#define GENESIS_LOG_THROW(logger, exception, type, msg, ...)\
{\
	GENESIS_LOG(logger, type, msg, __VA_ARGS__);\
	throw exception(msg);\
}

namespace genesis
{
	namespace logUtils {
		constexpr const char* getFilename(const char* path)
		{
			const char* file = path;

			while (*path) {
				if (*path == '/' || *path == '\\') {
					file = path + 1;
				}
				path++;
			}
			return file;
		}
	}
	
}

#endif