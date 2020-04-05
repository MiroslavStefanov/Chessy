#include "stdafx.h"
#include "Pawn.h"
#include "Utils/Utils.h"

#define BLACK_POSITION(i) TilePosition(CHESS_BOARD_SIDE + i)
#define WHITE_POSITION(i) TilePosition((CHESS_BOARD_SIDE - 2) * CHESS_BOARD_SIDE + i)

#define ITERATE_1(macro) macro(1), macro(0)
#define ITERATE_2(macro) macro(2), ITERATE_1(macro)
#define ITERATE_3(macro) macro(3), ITERATE_2(macro)
#define ITERATE_4(macro) macro(4), ITERATE_3(macro)
#define ITERATE_5(macro) macro(5), ITERATE_4(macro)
#define ITERATE_6(macro) macro(6), ITERATE_5(macro)
#define ITERATE_7(macro) macro(7), ITERATE_6(macro)

namespace chess
{
	namespace pieces
	{
		const ChessPiece Pawn =
		{
			8
			, 'p'
			, { ITERATE_7(WHITE_POSITION) }
			, { ITERATE_7(BLACK_POSITION) }
			, {
				  { EDirection::Forward, EMoveType::PawnMove }
				, { EDirection::Forward, EMoveType::PawnJump }
				, { EDirection::ForwardLeft, EMoveType::PawnHit }
				, { EDirection::ForwardRight, EMoveType::PawnHit }
			}
		};
	}

		
}