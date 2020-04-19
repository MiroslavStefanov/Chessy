#pragma once
#include "ChessPieces/ChessPiece.h"

namespace chess
{
	enum class EChessPieceType;
	enum class EColor;
	class TilePosition;

	class ChessPieceRegistry
	{
	public:
		std::size_t							GetInstancesCount(EChessPieceType type) const;
		const TilePosition&					GetInitialPosition(EChessPieceType type, EColor color, std::size_t instanceNumber) const;
		char								GetVisualRepresentation(EChessPieceType type, EColor color) const;
		const std::vector<ChessPieceMove>&	GetGameMoves(EChessPieceType type) const;

	private:
		const ChessPiece* GetPieceOfType(EChessPieceType type) const;
	};
}

