#include "Utils.h"
#include "../ChessPieces/ChessPiece.h"

namespace chess
{
	EDirection GetOpositeDirection(EDirection direction)
	{
		switch (direction)
		{
		case chess::ForwardLeft:
			return BackwardRight;
		case chess::Forward:
			return Backward;
		case chess::ForwardRight:
			return BackwardLeft;
		case chess::Right:
			return Left;
		case chess::BackwardRight:
			return ForwardLeft;
		case chess::Backward:
			return Forward;
		case chess::BackwardLeft:
			return ForwardRight;
		case chess::Left:
			return Right;
		default:
			return NoDirection;
		}
	}

	int8_t GetKingPosition(EColor color)
	{
		return color == White ? 4 : 5;
	}
}