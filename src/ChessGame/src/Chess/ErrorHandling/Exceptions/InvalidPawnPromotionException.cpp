#include "stdafx.h"
#include "InvalidPawnPromotionException.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	InvalidPawnPromotionException::InvalidPawnPromotionException(const std::exception& cause) noexcept
		: LoggedException("Invalid pawn promotion", cause)
	{
	}

}