#pragma once
#include "exception/LoggedException.h"

namespace chess
{
	class InvalidPlayerColorException : public mvc::LoggedException
	{
	public:
		explicit InvalidPlayerColorException(const std::exception& cause) noexcept;
	};
}