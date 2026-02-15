#ifndef GENESIS_LOG_UTILS_H
#define GENESIS_LOG_UTILS_H
#include <stdexcept>
#include <string>

#define GENESIS_LOG(logger, type, msg)\
logger.log((type), msg)
	
#define GENESIS_LOG_THROW(logger, exception, type, msg)\
{\
	GENESIS_LOG(logger, type, msg);\
	throw exception(msg);\
}

#define GENESIS_LOG_INFO(msg)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Info, msg)

#define GENESIS_LOG_WARNING(msg)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Warning, msg)

#define GENESIS_LOG_ERROR(msg)\
GENESIS_LOG(getLogger(), Logger::LogLevel::Error, msg)

#define GENESIS_LOG_THROW_ERROR(msg)\
GENESIS_LOG_THROW(getLogger(), std::runtime_error, Logger::LogLevel::Error, msg)

#define GENESIS_LOG_THROW_INVALID_ARG(msg)\
GENESIS_LOG_THROW(getLogger(), std::invalid_argument, Logger::LogLevel::Error, msg)

#endif