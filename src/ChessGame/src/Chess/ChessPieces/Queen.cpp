#include "stdafx.h"
#include "Queen.h"
#include "Utils/Utils.h"

namespace chess
{
	namespace pieces
	{
		const ChessPiece Queen =
		{
			1
			, 'q'
			, { TilePosition(CHESS_BOARD_SIDE * (CHESS_BOARD_SIDE - 1) + 4) }
			, { TilePosition(4) }
			, {
				  { EDirection::Forward, EMoveType::Multiple }
				, { EDirection::Backward, EMoveType::Multiple }
				, { EDirection::Left, EMoveType::Multiple }
				, { EDirection::Right, EMoveType::Multiple }
				, { EDirection::ForwardLeft, EMoveType::Multiple }
				, { EDirection::ForwardRight, EMoveType::Multiple }
				, { EDirection::BackwardLeft, EMoveType::Multiple }
				, { EDirection::BackwardRight, EMoveType::Multiple }
			}
		};
	}
}