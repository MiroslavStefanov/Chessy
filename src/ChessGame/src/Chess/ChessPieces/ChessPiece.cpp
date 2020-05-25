#include "stdafx.h"
#include "ChessPiece.h"

#define BLACK_PAWN_POSITION(i) TilePosition(CHESS_BOARD_SIDE + i)
#define WHITE_PAWN_POSITION(i) TilePosition((CHESS_BOARD_SIDE - 2) * CHESS_BOARD_SIDE + i)

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
		////////////////////////////////////////////////////////////////////////////////////////////////
		const ChessPiece Bishop =
		{
			2
			, { TilePosition(CHESS_BOARD_SIDE * (CHESS_BOARD_SIDE - 1) + 2), TilePosition(CHESS_BOARD_SIDE * CHESS_BOARD_SIDE - 3) }
			, { TilePosition(2), TilePosition(CHESS_BOARD_SIDE - 3) }
			, {
				  { EDirection::ForwardLeft, EMoveType::Multiple }
				, { EDirection::ForwardRight, EMoveType::Multiple }
				, { EDirection::BackwardLeft, EMoveType::Multiple }
				, { EDirection::BackwardRight, EMoveType::Multiple }
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////
		const ChessPiece King =
		{
			1
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
				, { EDirection::Left, EMoveType::Castle }
				, { EDirection::Right, EMoveType::Castle }
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////
		const ChessPiece Knight =
		{
			2
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

		////////////////////////////////////////////////////////////////////////////////////////////////
		const ChessPiece Pawn =
		{
			8
			, { ITERATE_7(WHITE_PAWN_POSITION) }
			, { ITERATE_7(BLACK_PAWN_POSITION) }
			, {
				  { EDirection::Forward, EMoveType::PawnMove }
				, { EDirection::Forward, EMoveType::PawnJump }
				, { EDirection::ForwardLeft, EMoveType::PawnHit }
				, { EDirection::ForwardRight, EMoveType::PawnHit }
			}
		};

		////////////////////////////////////////////////////////////////////////////////////////////////
		const ChessPiece Queen =
		{
			1
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

		const ChessPiece Rook =
		{
			2
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