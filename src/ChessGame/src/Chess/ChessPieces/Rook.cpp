#include "stdafx.h"
#include "Rook.h"
#include "Utils/Utils.h"

namespace chess
{
	namespace pieces
	{
		const ChessPiece Rook =
		{
			2
			, 'r'
			, { TilePosition(CHESS_BOARD_SIDE * (CHESS_BOARD_SIDE - 1)),  TilePosition(CHESS_BOARD_SIDE * CHESS_BOARD_SIDE - 1) }
			, { TilePosition(0), TilePosition(CHESS_BOARD_SIDE - 1) }
			, {
				  { EDirection::Forward, EMoveType::Multiple }
				, { EDirection::Backward, EMoveType::Multiple }
				, { EDirection::Left, EMoveType::Multiple }
				, { EDirection::Right, EMoveType::Multiple }
			}
		};
	}
}