#include "stdafx.h"
#include "Utils.h"
#include "../ChessPieces/ChessPiece.h"

namespace chess
{
	EColor GetAlternateColor(EColor color)
	{
		switch (color)
		{
		case EColor::White:
			return EColor::Black;
		case EColor::Black:
			return EColor::White;
		default:
			return EColor::Colorless;
		}
	}

	bool IsPawnPromotionPosition(EColor pawnColor, const TilePosition& position)
	{
		switch (pawnColor)
		{
		case chess::EColor::White:
			return position.AsPosition().row == 0;
		case chess::EColor::Black:
			return position.AsPosition().row == CHESS_BOARD_SIDE - 1;
		default:
			assert(false);
			return false;
		}
	}

	bool IsValidCastleDirection(EDirection direction)
	{
		return direction == EDirection::Left || direction == EDirection::Right;
	}

	bool TilePosition::IsValid() const
	{
		const auto position = AsPosition();
		const bool validRow = position.row >= 0 && position.row < CHESS_BOARD_SIDE;
		const bool validColumn = position.col >= 0 && position.col < CHESS_BOARD_SIDE;
		return validRow && validColumn;
	}

	const ChessPieceMove& ChessPieceMove::Invalid()
	{
		static const ChessPieceMove invalidMove(EDirection::COUNT, EMoveType::Error);
		return invalidMove;
	}

	ChessPieceMove::ChessPieceMove(EDirection direction, EMoveType type)
		: m_direction(direction)
		, m_type(type)
	{
	}

	bool ChessPieceMove::IsValid() const
	{
		return m_direction != EDirection::COUNT && m_type != EMoveType::Error;
	}

	EDirection ChessPieceMove::GetDirection() const
	{
		return m_direction;
	}

	EMoveType ChessPieceMove::GetMoveType() const
	{
		return m_type;
	}
}