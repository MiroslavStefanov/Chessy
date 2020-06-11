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
		const std::vector<ChessPieceId>& GetTilePositionHitters(const TilePosition& position) const;
		const TilePositionSet& GetPlayerPossibleMoves() const;
		void InvalidatePlayerPossibleMovesCache();

	private:
		void RemoveHitterFromPosition(ChessPieceId hitter, const TilePosition& position);
		TilePositionSet CollectPlayerPossibleMoves() const;

	private:
		std::unordered_map<ChessPieceId, std::vector<TilePosition>, ChessPieceIdHash> m_possibleMoves;
		std::unordered_map<TilePosition, std::vector<ChessPieceId>, TilePositionHash> m_positionHitters;

		mutable struct
		{
			TilePositionSet m_playerMoves;
			bool m_isDirty = true;
		} m_Cache;
	};
}

