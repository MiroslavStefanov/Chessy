#include "stdafx.h"
#include "ChessPieceRegistry.h"
#include "Utils/Utils.h"

#include "ChessPieces/ChessPiece.h"
#include "ChessPieces/ChessPieceTypes.h"

#define REGISTER_CHESS_PIECE(chessPieceType) { EChessPieceType::chessPieceType, &pieces::chessPieceType },

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	std::size_t ChessPieceRegistry::GetInstancesCount(EChessPieceType type)
	{
		if (auto piece = CHESS_PIECES_TABLE.at(type))
			return piece->InstacesCount;

		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& ChessPieceRegistry::GetInitialPosition(EChessPieceType type, EColor color, std::size_t instanceNumber)
	{
		if (auto piece = CHESS_PIECES_TABLE.at(type))
		{
			if (instanceNumber < piece->InstacesCount)
			{
				if (color == EColor::Black)
					return piece->BlackInitialPositions[instanceNumber];
				if (color == EColor::White)
					return piece->WhiteInitialPositions[instanceNumber];
			}
		}
		return TilePosition::Invalid();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<ChessPieceMove>& ChessPieceRegistry::GetGameMoves(EChessPieceType type)
	{
		static const std::vector<ChessPieceMove> empty;
		if (auto piece = CHESS_PIECES_TABLE.at(type))
		{
			return piece->GameMoves;
		}
		return empty;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const std::unordered_map<EChessPieceType, const ChessPiece*> ChessPieceRegistry::CHESS_PIECES_TABLE =
	{
		CHESS_PIECE_TYPE_ITERATOR(REGISTER_CHESS_PIECE)
		{ EChessPieceType::COUNT, nullptr }
	};
}