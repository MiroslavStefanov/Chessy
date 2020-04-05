#include "stdafx.h"
#include "King.h"
#include "Utils/Utils.h"

namespace chess
{
	namespace pieces
	{
		const ChessPiece King =
		{
			1
			, 'w'
			, { TilePosition(CHESS_BOARD_SIDE * (CHESS_BOARD_SIDE - 1) + 3) }
			, { TilePosition(3) }
			, {
				  { EDirection::ForwardLeft, EMoveType::KingMove }
				, { EDirection::Forward, EMoveType::KingMove }
				, { EDirection::ForwardRight, EMoveType::KingMove }
				, { EDirection::Right, EMoveType::KingMove }
				, { EDirection::BackwardRight, EMoveType::KingMove }
				, { EDirection::Backward, EMoveType::KingMove }
				, { EDirection::BackwardLeft, EMoveType::KingMove }
				, { EDirection::Left, EMoveType::KingMove }
				, { EDirection::NoDirection, EMoveType::Castle }
			}
		};
	}
}
