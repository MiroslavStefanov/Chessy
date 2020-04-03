#include "Queen.h"
#include "../Utils/Utils.h"

namespace chess
{
	Queen::Queen(EColor color) :
		ChessPiece(EChessPieceType::QueenType, color),
		Rook(color),
		Bishop(color)
	{
	}
}