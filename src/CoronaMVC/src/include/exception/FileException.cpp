#include "pch.h"
#include "FileException.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	FileException::FileException(std::string message) noexcept
		: LoggedException(message)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	FileException::FileException(std::string message, const std::exception& cause) noexcept
		: LoggedException(message, cause)
	{
	}

}