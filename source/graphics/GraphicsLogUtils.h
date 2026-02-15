#ifndef GENESIS_GRAPHICS_LOG_UTILS_H
#define GENESIS_GRAPHICS_LOG_UTILS_H
#include <core/Logger.h>
#include <d3d11.h>

#define GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(res, msg)\
{\
	HRESULT result = (res);\
	if (FAILED(result)) {\
		GENESIS_LOG_THROW_ERROR(msg);\
	}\
}

#define GENESIS_GRAPHICS_CHECK_SHADER_COMPILE(res, error)\
{\
	HRESULT result = (res);\
	genesis::graphicsLogUtils::checkShaderCompile(getLogger(), result, error);\
}

namespace genesis
{
	namespace graphicsLogUtils
	{
		inline void checkShaderCompile(Logger& logger, HRESULT res, ID3DBlob* error)
		{
			const char* errorMsg = error ? static_cast<const char*>(error->GetBufferPointer()) : nullptr;

			if (FAILED(res)) {
				if (errorMsg) {
					GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, errorMsg);
				}
				else {
					GENESIS_LOG_THROW(logger, std::runtime_error, Logger::LogLevel::Error, "Shader compilation failed.");
				}
			}

			if (errorMsg) {
				GENESIS_LOG(logger, Logger::LogLevel::Warning, errorMsg);
			}
		}
	}
}

#endif