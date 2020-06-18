#include "stdafx.h"
#include "InvalidPlayerColorException.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	InvalidPlayerColorException::InvalidPlayerColorException(const std::exception& cause) noexcept
		: LoggedException("Invalid player color", cause)
	{
	}

}