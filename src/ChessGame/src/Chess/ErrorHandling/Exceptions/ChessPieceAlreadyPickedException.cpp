#include "stdafx.h"
#include "ChessPieceAlreadyPickedException.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceAlreadyPickedException::ChessPieceAlreadyPickedException() noexcept
		: LoggedException("There is already picked chess piece")
	{
	}

}