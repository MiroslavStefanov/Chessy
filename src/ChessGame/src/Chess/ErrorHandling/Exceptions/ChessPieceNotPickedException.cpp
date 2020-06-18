#include "stdafx.h"
#include "ChessPieceNotPickedException.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceNotPickedException::ChessPieceNotPickedException() noexcept
		: LoggedException("There is no picked chess piece")
	{
	}

}