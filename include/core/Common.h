#ifndef GENESIS_COMMON_H
#define GENESIS_COMMON_H
#include <core/Core.h>
#include <core/Logger.h>
#include <math/Rect.h>

#define GENESIS_DEFAULT_WINDOW_WIDTH 1280
#define GENESIS_DEFAULT_WINDOW_HEIGHT 720

#if _WIN32
#define GENESIS_TEXT(string) L##string
#else
#define GENESIS_TEXT(string) string
#endif

namespace genesis 
{
	struct BaseDesc
	{
		Logger& logger;
	};

	struct GameDesc
	{
		Rect windowSize{GENESIS_DEFAULT_WINDOW_WIDTH, GENESIS_DEFAULT_WINDOW_HEIGHT};
		Logger::LogLevel logLevel = Logger::LogLevel::Error;
	};

	struct WindowDesc
	{
		BaseDesc base;
		Rect size{};
#ifdef _WIN32
		const wchar_t* title{};
#else
		const char* title{};
#endif
	};

	struct DisplayDesc
	{
		WindowDesc window;
		GraphicsDevice& graphicsDevice;
	};

	struct GraphicsEngineDesc
	{
		BaseDesc base;
	};

	struct GraphicsDeviceDesc
	{
		BaseDesc base;
	};

	struct SwapChainDesc
	{
		void* windowHandle{};
		Rect windowSize{};
	};

	enum class ShaderType
	{
		VertexShader,
		PixelShader
	};

	struct ShaderCompileDesc
	{
		const char* shaderSourceName{};
		const void* shaderSourceCode{};
		size_t shaderSourceCodeSize{};
		const char* shaderEntryPoint{};
		ShaderType shaderType{};
	};

	struct ShaderBinaryData
	{
		const void* data{};
		size_t dataSize{};
	};

	struct GraphicsPipelineStateDesc
	{
		const ShaderBinary& vertexShader;
		const ShaderBinary& pixelShader;
	};

	struct VertexBufferDesc
	{
		const void* vertexList{};
		size_t vertexListSize{};
		unsigned int vertexSize{};
	};

}

#endif