#pragma once
#include "ChessPieces/Movement/ChessPieceMovementIterator.h"

namespace chess
{
	class PawnMovementIterator : public ChessPieceMovementIterator
	{
	public:
		PawnMovementIterator(const std::vector<ChessPieceId>& boardState, const TilePosition& initialPosition, const TilePosition& enPassantPosition);

	protected:
		void TryMove(const ChessPieceMove& move) override;

	private:
		bool CanDoPawnMove(const TilePosition& position) const;
		bool CanDoPawnJump(const TilePosition& position) const;
		bool CanDoPawnHit(const TilePosition& position) const;

	private:
		const TilePosition m_enPassantPosition;
	};
}

