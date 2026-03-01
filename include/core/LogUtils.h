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

#define GENESIS_LOG_INFO(msg, ...)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Info, msg, __VA_ARGS__)

#define GENESIS_LOG_WARNING(msg, ...)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Warning, msg, __VA_ARGS__)

#define GENESIS_LOG_ERROR(msg, ...)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Error, msg, __VA_ARGS__)

#define GENESIS_LOG_THROW_ERROR(msg, ...)\
GENESIS_LOG_THROW(getLogger(), std::runtime_error, Logger::LogLevel::Error, msg, __VA_ARGS__)

#define GENESIS_LOG_THROW_INVALID_ARG(msg, ...)\
GENESIS_LOG_THROW(getLogger(), std::invalid_argument, Logger::LogLevel::Error, msg, __VA_ARGS__)

#endif