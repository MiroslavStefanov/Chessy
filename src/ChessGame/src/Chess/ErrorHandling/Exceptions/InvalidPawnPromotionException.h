#pragma once
#include "exception/LoggedException.h"

namespace chess
{
	class InvalidPawnPromotionException : public mvc::LoggedException
	{
	public:
		explicit InvalidPawnPromotionException(const std::exception& cause) noexcept;
	};
}