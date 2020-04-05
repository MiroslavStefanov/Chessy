#include "stdafx.h"
#include "ConsoleChessInputDevice.h"
#include "Utils/Utils.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	TilePosition ConsoleChessInputDevice::PollTilePosition()
	{
		int row, col;
		std::cin >> row >> col;
		return TilePosition(Position(row, col));
	}
}