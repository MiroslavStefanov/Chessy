#pragma once
#include "exception/LoggedException.h"

namespace chess
{
	class ChessPieceNotPickedException : public mvc::LoggedException
	{
	public:
		ChessPieceNotPickedException() noexcept;
	};
}