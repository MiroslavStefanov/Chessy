#include "stdafx.h"
#include "ConsoleChessInputDevice.h"
#include "ConsoleChessVisuals.h"
#include "ChessPieces/ChessPieceTypes.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ConsoleChessInputDevice::ConsoleChessInputDevice() 
		: m_inputPosition(TilePosition::Invalid())
		, m_inputPieceType(EChessPieceType::COUNT)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessInputDevice::Update()
	{
		std::string line;
		std::getline(std::cin, line);
		m_inputPosition = ParseTilePosition(line);

		if (!m_inputPosition.IsValid())
		{
			m_inputPieceType = ParseChessPieceType(line);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ConsoleChessInputDevice::HasAnyInput() const
	{
		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& ConsoleChessInputDevice::GetTilePosition() const
	{
		return m_inputPosition;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	EChessPieceType ConsoleChessInputDevice::GetChessPieceType() const
	{
		return m_inputPieceType;
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

	////////////////////////////////////////////////////////////////////////////////////////////////
	EChessPieceType ConsoleChessInputDevice::ParseChessPieceType(const std::string& input) const
	{
#define PARSE_CHESS_PICECE_TYPE(type) \
		if (CaseInsensitiveEquals(input, #type)) \
		{ \
			return EChessPieceType::type; \
		} \

		CHESS_PIECE_TYPE_ITERATOR(PARSE_CHESS_PICECE_TYPE);

#undef PARSE_CHESS_PIECE_TYPE

		return EChessPieceType::COUNT;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ConsoleChessInputDevice::CaseInsensitiveEquals(const std::string& lhs, const std::string& rhs) const
	{
		return std::equal(
			lhs.begin(), 
			lhs.end(), 
			rhs.begin(), 
			[](char leftChar, char rightChar) {return std::tolower(leftChar) == std::tolower(rightChar); }
		);
	}
}