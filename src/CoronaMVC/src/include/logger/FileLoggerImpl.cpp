#include "pch.h"
#include "FileLoggerImpl.h"
#include "logger/LogSeverity.h"
#include "exception/FileException.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	FileLoggerImpl::FileLoggerImpl(const std::string& fileName)
	{
		m_outStream = std::ofstream(fileName, std::ios::out | std::ios::trunc);
		if (!m_outStream)
		{
			throw FileException(std::string("Could not open file for logging: ") + fileName);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void FileLoggerImpl::PrintLog(const std::string& log, ELogSeverity severity)
	{
		PrintSeverity(severity);
		m_outStream << log << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void FileLoggerImpl::PrintSeverity(ELogSeverity severity)
	{
		static constexpr auto SEVERITY_START = '[';
		static constexpr auto SEVERITY_END = ']';

		m_outStream << SEVERITY_START;
		switch (severity)
		{
		case ELogSeverity::Message:
			m_outStream << "Message";
			break;
		case ELogSeverity::Warning:
			m_outStream << "Warning";
			break;
		case ELogSeverity::Error:
			m_outStream << "Error";
			break;
		default:
			m_outStream << "Invalid severity code: " << (int)severity;
			break;
		}
		m_outStream << SEVERITY_END;
	}
}