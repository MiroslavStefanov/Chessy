#include "stdafx.h"
#include "ConsoleChessInputDevice.h"
#include "Events/CellClickedEvent.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessInputDevice::CollectInputEvents()
	{
		int row, col;
		std::cin >> row >> col;
		AddEvent(std::make_unique<CellClickedEvent>(chess::TilePosition(chess::Position(row, col))));
	}
}