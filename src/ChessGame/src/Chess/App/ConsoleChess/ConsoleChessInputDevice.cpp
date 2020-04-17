#include "stdafx.h"
#include "ConsoleChessInputDevice.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ConsoleChessInputDevice::ConsoleChessInputDevice() : m_inputPosition(TilePosition::Invalid())
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessInputDevice::Update()
	{
		int row, col;
		std::cin >> row >> col;
		m_inputPosition = TilePosition(Position(row, col));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& ConsoleChessInputDevice::GetInputTilePosition()
	{
		return m_inputPosition;
	}
}