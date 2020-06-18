#pragma once
#include "exception/LoggedException.h"

namespace chess
{
	class InvalidChessPieceMoveException : public mvc::LoggedException
	{
	public:
		explicit InvalidChessPieceMoveException(const std::exception& cause) noexcept;
	};
}