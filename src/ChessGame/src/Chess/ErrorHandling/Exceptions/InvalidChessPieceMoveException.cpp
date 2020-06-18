#include "stdafx.h"
#include "InvalidChessPieceMoveException.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	InvalidChessPieceMoveException::InvalidChessPieceMoveException(const std::exception& cause) noexcept
		: LoggedException("Invalid chess piece move", cause)
	{
	}

}