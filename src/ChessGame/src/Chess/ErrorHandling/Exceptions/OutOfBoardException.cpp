#include "stdafx.h"
#include "OutOfBoardException.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	OutOfBoardException::OutOfBoardException(const TilePosition& position) noexcept
		: LoggedException("Chess tile position out of board with row = " 
			+ std::to_string(position.AsPosition().row)
			+ ", column = " 
			+ std::to_string(position.AsPosition().col))
	{
	}
}