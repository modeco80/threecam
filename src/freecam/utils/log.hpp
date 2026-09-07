#ifndef UTILS_LOG_HPP
#define UTILS_LOG_HPP

enum LogLevel {
	LogInfo,
	LogWarn,
	LogErr
};

void utilLog(LogLevel level, const char* pszMessage);

void utilLogf(LogLevel level, const char* pszMessage, ...);

#endif
