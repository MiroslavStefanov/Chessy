#include "pch.h"
#include "LoggedException.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	LoggedException::LoggedException(std::string message) noexcept
		: std::runtime_error(message)
		, m_cause(nullptr)
	{
		m_message = runtime_error::what();
		Logger::Get().LogError("Exception thrown: {0}", m_message);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	LoggedException::LoggedException(std::string message, const std::exception& cause) noexcept
		: std::runtime_error(std::move(message))
		, m_cause(&cause)
	{
		m_message = runtime_error::what();
		m_message += std::string("Caused by excpetion: ") + cause.what();
		Logger::Get().LogError("Exception thrown: {0}", m_message);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const char* LoggedException::what() const noexcept
	{
		return m_message.c_str();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const std::exception* LoggedException::GetCause() const
	{
		return m_cause;
	}

}