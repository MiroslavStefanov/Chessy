#pragma once
#include "Utils/Utils.h"
#include "ChessPieces/ChessPieceTypes.h"

#define DECLARE_CHESS_PIECE(chessPieceType) extern const ChessPiece chessPieceType;

namespace chess
{
	struct ChessPiece
	{
		std::size_t InstacesCount;
		std::vector<TilePosition> WhiteInitialPositions;
		std::vector<TilePosition> BlackInitialPositions;
		std::vector<ChessPieceMove> GameMoves;
	};

	namespace pieces
	{
		CHESS_PIECE_TYPE_ITERATOR(DECLARE_CHESS_PIECE)
	}
}

#undef DECLARE_CHESS_PIECE
