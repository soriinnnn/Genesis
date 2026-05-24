#ifndef GENESIS_LOG_UTILS_H
#define GENESIS_LOG_UTILS_H
#include <core/Core.h>

#define GENESIS_LOG(logger, type, msg, ...)\
logger.log((type), msg __VA_OPT__(,) __VA_ARGS__)
	
#define GENESIS_LOG_THROW(logger, exception, type, msg, ...)\
{\
	GENESIS_LOG(logger, type, msg __VA_OPT__(,) __VA_ARGS__);\
	throw exception(std::format(msg __VA_OPT__(,) __VA_ARGS__));\
}

#define GENESIS_LOG_INFO(msg, ...)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Info, msg __VA_OPT__(,) __VA_ARGS__)

#define GENESIS_LOG_WARNING(msg, ...)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Warning, msg __VA_OPT__(,) __VA_ARGS__)

#define GENESIS_LOG_ERROR(msg, ...)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Error, msg __VA_OPT__(,) __VA_ARGS__)

#define GENESIS_LOG_THROW_ERROR(msg, ...)\
GENESIS_LOG_THROW(getLogger(), std::runtime_error, Logger::LogLevel::Error,\
"\b[{}:{}] " msg, genesis::logUtils::getFilename(__FILE__), __LINE__ __VA_OPT__(,) __VA_ARGS__)

#define GENESIS_LOG_THROW_INVALID_ARG(msg, ...)\
GENESIS_LOG_THROW(getLogger(), std::invalid_argument, Logger::LogLevel::Error,\
"\b[{}:{}] " msg, genesis::logUtils::getFilename(__FILE__), __LINE__ __VA_OPT__(,) __VA_ARGS__)

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