#pragma once
#include "LoggedException.h"

namespace mvc
{
	class FileException : public LoggedException
	{
	public:
		explicit FileException(std::string message) noexcept;
		explicit FileException(std::string message, const std::exception& cause) noexcept;
	};
}