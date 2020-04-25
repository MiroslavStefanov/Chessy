#pragma once
#include "Utils/Utils.h"

namespace chess
{
	enum class EChessPieceType;
	enum class EColor;
	class TilePosition;
	struct ChessPiece;

	class ChessPieceRegistry
	{
	public:
		static std::size_t GetInstancesCount(EChessPieceType type);
		static const TilePosition& GetInitialPosition(EChessPieceType type, EColor color, std::size_t instanceNumber);
		static const std::vector<ChessPieceMove>& GetGameMoves(EChessPieceType type);

	private:
		static const std::unordered_map<EChessPieceType, const ChessPiece*> CHESS_PIECES_TABLE;
	};
}

