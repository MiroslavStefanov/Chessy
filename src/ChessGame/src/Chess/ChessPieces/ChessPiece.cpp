#include "stdafx.h"
#include "ChessPiece.h"

#include "ChessBoard.h"
#include "ChessboardTile.h"

#include "Utils/Utils.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPiece::ChessPiece(EChessPieceType type, EColor color) :
		m_type(type), m_color(color), m_ownerTilePosition(TilePosition(0)), m_hasMoved(false)
	{

	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void ChessPiece::SetOwnerTilePosition(const TilePosition& ownerTilePosition, bool isInit)
	{
		m_ownerTilePosition = ownerTilePosition;
		if (isInit == false)
		{
			m_hasMoved = true;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void ChessPiece::SetPossibleGameMoves(const std::vector<TilePosition>& possibleGameMoves)
	{
		m_possibleMoves.clear();
		m_possibleMoves = possibleGameMoves;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EChessPieceType ChessPiece::GetType() const
	{
		return m_type;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EColor ChessPiece::GetColor() const
	{
		return m_color;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<TilePosition>& ChessPiece::GetPossibleGameMoves() const
	{
		return m_possibleMoves;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool ChessPiece::IsPossibleMove(const TilePosition& position) const
	{
		auto it = std::find_if(m_possibleMoves.begin(), m_possibleMoves.end(), [&position](const TilePosition& pos) {
			return pos == position;
			});
		return it != m_possibleMoves.end();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<ChessPieceMove>& ChessPiece::GetChessPieceMoves() const
	{
		return m_chessMoves;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& ChessPiece::GetOnwerTilePosition() const
	{
		return m_ownerTilePosition;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool ChessPiece::HasMoved() const
	{
		return m_hasMoved;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::string ChessPiece::GetDrawString()
	{
		char drawChar = '0' + (int)m_type;
		return std::string() + drawChar;
	}
}