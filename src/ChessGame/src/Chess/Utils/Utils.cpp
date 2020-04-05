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
			return EDirection::NoDirection;
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
}