#ifndef GENESIS_GRAPHICS_LOG_UTILS_H
#define GENESIS_GRAPHICS_LOG_UTILS_H
#include <core/Logger.h>
#include <d3d11.h>

#define GENESIS_GRAPHICS_LOG_THROW_ON_FAIL(res, msg, ...)\
{\
	HRESULT result = (res);\
	if (FAILED(result)) {\
		GENESIS_LOG_THROW_ERROR(msg "\nError code: 0x{:08X}", __VA_OPT__(__VA_ARGS__,) static_cast<uint32>(result));\
	}\
}

#define GENESIS_GRAPHICS_CHECK_SHADER_COMPILE(res, error)\
{\
	HRESULT result = (res);\
	genesis::graphicsLogUtils::checkShaderCompile(getLogger(), result, error);\
}

GENESIS_NAMESPACE_START

namespace graphicsLogUtils
{
	inline void checkShaderCompile(Logger& logger, HRESULT res, ID3DBlob* error)
	{
		const char* errorMsg = error ? static_cast<const char*>(error->GetBufferPointer()) : nullptr;

		if (FAILED(res)) {
			GENESIS_LOG_THROW(
				logger,
				std::runtime_error,
				Logger::LogLevel::Error,
				"Shader compilation failed.\nDetails:\n{}",
				errorMsg
			);
		}

		if (errorMsg) {
			GENESIS_LOG(logger, Logger::LogLevel::Warning, "Shader compiled with warnings.\nDetails:\n{}", errorMsg);
		}
	}
}

GENESIS_NAMESPACE_END

#endif