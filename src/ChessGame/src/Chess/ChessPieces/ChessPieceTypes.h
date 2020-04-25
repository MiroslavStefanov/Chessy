#pragma once
#include "Utils/Utils.h"

#define CHESS_PIECE_TYPE_ITERATOR(macro) \
	macro(Bishop) \
	macro(King) \
	macro(Knight) \
	macro(Pawn) \
	macro(Queen) \
	macro(Rook)

#define ENUMERATE_CHESS_PIECE_TYPE(chessPieceType) chessPieceType,

namespace chess
{
	enum class EChessPieceType
	{
		CHESS_PIECE_TYPE_ITERATOR(ENUMERATE_CHESS_PIECE_TYPE)
		COUNT
	};
}

#undef ENUMERATE_CHESS_PIECE_TYPE
