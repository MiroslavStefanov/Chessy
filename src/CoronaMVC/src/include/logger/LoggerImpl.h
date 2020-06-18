#pragma once

namespace mvc
{
	enum class ELogSeverity;
	class LoggerImpl
	{
	public:
		virtual ~LoggerImpl() = default;
		virtual void PrintLog(const std::string& log, ELogSeverity severity) = 0;
	};
}