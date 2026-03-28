#ifndef GENESIS_LOGGER_H
#define GENESIS_LOGGER_H
#include <core/Core.h>
#include <core/utils/Macros.h>

namespace genesis
{
	class Logger final
	{
	GENESIS_DISABLE_COPY_AND_MOVE(Logger)
	public:
		enum class LogLevel
		{
			Error,
			Warning,
			Info
		};

		explicit Logger(LogLevel level = LogLevel::Error);
		~Logger();

		template<typename... Args>
		void log(LogLevel level, std::format_string<Args...> format, Args&&... args) const
		{
			std::string msg = std::format(format, std::forward<Args>(args)...);
			_log(level, msg.c_str());
		}

	private:
		void _log(LogLevel level, const char* msg) const;

	private:
		LogLevel m_level;
	};
}

#endif