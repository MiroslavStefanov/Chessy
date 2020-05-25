#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class ChessPieceMovementIterator;

	class PlayerPossibleMoves
	{
	public:
		void UpdateChessPiecePossibleMoves(ChessPieceId pieceId, std::unique_ptr<ChessPieceMovementIterator>&& pieceMovementIterator);
		void RemoveChessPiecePossibleMoves(ChessPieceId pieceId);

		const std::vector<TilePosition>& GetChessPiecePossibleMoves(ChessPieceId pieceId) const;
		TilePositionSet GetPlayerPossibleMoves() const;

	private:
		std::unordered_map<ChessPieceId, std::vector<TilePosition>, ChessPieceIdHash> m_possibleMoves;

	};
}

