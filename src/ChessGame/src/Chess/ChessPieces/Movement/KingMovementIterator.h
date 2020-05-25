#pragma once
#include "ChessPieces/Movement/ChessPieceMovementIterator.h"

namespace chess
{
	class PlayerCastleCache;
	class KingMovementIterator : public ChessPieceMovementIterator
	{
	public:
		KingMovementIterator(const std::vector<ChessPieceId>& boardState, const TilePosition& initialPosition, TilePositionSet&& checkPositions, const PlayerCastleCache& castleCache);

	protected:
		void TryMove(const ChessPieceMove& move) override;

	private:
		void TryCastle(EDirection direction);

	private:
		const TilePositionSet m_checkPositions;
		const PlayerCastleCache& m_castleCache;
	};
}

