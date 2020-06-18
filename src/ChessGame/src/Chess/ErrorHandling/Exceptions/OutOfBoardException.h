#pragma once
#include "exception/LoggedException.h"
#include "Utils/Utils.h"

namespace chess
{
	class OutOfBoardException : public mvc::LoggedException
	{
	public:
		explicit OutOfBoardException(const TilePosition& position) noexcept;
	};
}