#include <core/Logger.h>
#include <iostream>

using namespace genesis;
using namespace std;

static const char* logLevelToString(Logger::LogLevel level);

Logger::Logger(LogLevel level)
{
	m_level = level;
	clog << "Logger initialized(level: " << logLevelToString(m_level) << ")." << endl;
}

Logger::~Logger() {}

void Logger::_log(LogLevel level, const char* msg) const
{
	if (level > m_level) {
		return;
	}
	clog << "[" << logLevelToString(level) << "] " << msg << endl;
}

/* STATIC FUNCTION DEFINITIONS */

static const char* logLevelToString(Logger::LogLevel level) 
{
	switch (level) {
	case Logger::LogLevel::Info:
		return "Info";
	case Logger::LogLevel::Warning:
		return "Warning";
	case Logger::LogLevel::Error:
		return "Error";
	default: 
		return "Unknown";
	}
}