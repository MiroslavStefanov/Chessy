#pragma once
#include "logger/LoggerImpl.h"

namespace mvc
{
	class FileLoggerImpl : public LoggerImpl
	{
	public:
		explicit FileLoggerImpl(const std::string& fileName);
		virtual void PrintLog(const std::string& log, ELogSeverity severity);

	private:
		void PrintSeverity(ELogSeverity severity);

	private:
		std::ofstream m_outStream;
	};
}