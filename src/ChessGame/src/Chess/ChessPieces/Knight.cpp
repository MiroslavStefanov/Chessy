#include "stdafx.h"
#include "Knight.h"
#include "Utils/Utils.h"

namespace chess
{
	namespace pieces
	{
		const ChessPiece Knight =
		{
			2
			, 'k'
			, { TilePosition(CHESS_BOARD_SIDE * (CHESS_BOARD_SIDE - 1) + 1), TilePosition(CHESS_BOARD_SIDE * CHESS_BOARD_SIDE - 2) }
			, { TilePosition(1), TilePosition(CHESS_BOARD_SIDE - 2) }
			, {
				  { EDirection::ForwardLeft, EMoveType::KnightMove }
				, { EDirection::Forward, EMoveType::KnightMove }
				, { EDirection::ForwardRight, EMoveType::KnightMove }
				, { EDirection::Right, EMoveType::KnightMove }
				, { EDirection::BackwardRight, EMoveType::KnightMove }
				, { EDirection::Backward, EMoveType::KnightMove }
				, { EDirection::BackwardLeft, EMoveType::KnightMove }
				, { EDirection::Left, EMoveType::KnightMove }
			}
		};
	}
}