#pragma once
#include "ChessPieces/Movement/ChessPieceMovementIterator.h"

namespace chess
{
	class KingMovementIterator : public ChessPieceMovementIterator
	{
	public:
		KingMovementIterator(const std::vector<ChessPieceId>& boardState, const TilePosition& initialPosition);

	protected:
		void TryMove(const ChessPieceMove& move) override;
	};
}

