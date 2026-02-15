#ifndef GENESIS_LOGGER_H
#define GENESIS_LOGGER_H
#include <core/LogUtils.h>

namespace genesis
{
	class Logger final
	{
	public:
		enum class LogLevel
		{
			Error,
			Warning,
			Info
		};

		explicit Logger(LogLevel level = LogLevel::Error);
		~Logger();

		void log(LogLevel level, const char* msg);

	protected:
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator =(const Logger&) = delete;
		Logger& operator =(Logger&&) = delete;

	private:
		LogLevel m_level;
	};
}

#endif