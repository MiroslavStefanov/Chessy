#include "stdafx.h"
#include "ConsoleChessInputDevice.h"
#include "ConsoleChessVisuals.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ConsoleChessInputDevice::ConsoleChessInputDevice() : m_inputPosition(TilePosition::Invalid())
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessInputDevice::Update()
	{
		std::string line;
		std::getline(std::cin, line);
		m_inputPosition = ParseTilePosition(line);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& ConsoleChessInputDevice::GetInputTilePosition()
	{
		return m_inputPosition;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	TilePosition ConsoleChessInputDevice::ParseTilePosition(const std::string& input) const
	{
		int row = -1, column = -1;
		for (char c : input)
		{
			if (IsValidColumn(c))
			{
				if (column < 0)
				{
					column = ConsoleChessVisuals::ColumnIndexFromVisual(c);
				}
				else
				{
					return TilePosition::Invalid();
				}
			}
			else if (IsValidRow(c))
			{
				if (row < 0)
				{
					row = ConsoleChessVisuals::RowIndexFromVisual(c);
				}
				else
				{
					return TilePosition::Invalid();
				}
			}
		}
		return TilePosition(Position(row, column));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ConsoleChessInputDevice::IsValidRow(char row) const
	{
		return row >= '0' && row <= '9';
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ConsoleChessInputDevice::IsValidColumn(char column) const
	{
		column = std::toupper(column);
		return column >= 'A' && column <= 'H';
	}
}