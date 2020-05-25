#include "stdafx.h"
#include "Utils.h"
#include "../ChessPieces/ChessPiece.h"

namespace chess
{
	EDirection GetOpositeDirection(EDirection direction)
	{
		switch (direction)
		{
		case chess::EDirection::ForwardLeft:
			return EDirection::BackwardRight;
		case chess::EDirection::Forward:
			return EDirection::Backward;
		case chess::EDirection::ForwardRight:
			return EDirection::BackwardLeft;
		case chess::EDirection::Right:
			return EDirection::Left;
		case chess::EDirection::BackwardRight:
			return EDirection::ForwardLeft;
		case chess::EDirection::Backward:
			return EDirection::Forward;
		case chess::EDirection::BackwardLeft:
			return EDirection::ForwardRight;
		case chess::EDirection::Left:
			return EDirection::Right;
		default:
			return EDirection::COUNT;
		}
	}

	int8_t GetKingPosition(EColor color)
	{
		return color == EColor::White ? 4 : 5;
	}

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
		case chess::EColor::Colorless:
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