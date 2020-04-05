#include "stdafx.h"
#include "Bishop.h"

namespace chess
{
	namespace pieces
	{
		const ChessPiece Bishop =
		{
			2
			, 'b'
			, { TilePosition(CHESS_BOARD_SIDE * (CHESS_BOARD_SIDE - 1) + 2), TilePosition(CHESS_BOARD_SIDE * CHESS_BOARD_SIDE - 3) }
			, { TilePosition(2), TilePosition(CHESS_BOARD_SIDE - 3) }
			, {
				  { EDirection::ForwardLeft, EMoveType::Multiple }
				, { EDirection::ForwardRight, EMoveType::Multiple }
				, { EDirection::BackwardLeft, EMoveType::Multiple }
				, { EDirection::BackwardRight, EMoveType::Multiple }
			}
		};
	}
}