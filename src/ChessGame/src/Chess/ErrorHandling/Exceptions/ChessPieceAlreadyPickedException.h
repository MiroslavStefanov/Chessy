#pragma once
#include "exception/LoggedException.h"

namespace chess
{
	class ChessPieceAlreadyPickedException : public mvc::LoggedException
	{
	public:
		ChessPieceAlreadyPickedException() noexcept;
	};
}