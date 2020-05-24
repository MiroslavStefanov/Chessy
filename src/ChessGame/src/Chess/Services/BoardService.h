#pragma once
#include "Utils/Utils.h"
#include "Utils/EnPassantCache.h"

namespace chess
{
	class ChessPieceMovementIterator;

	class BoardService
	{
	public:
		BoardService();

		const std::vector<ChessPieceId>& GetBoardState() const;
		std::unique_ptr<ChessPieceMovementIterator> CreatePossibleMovesIterator(ChessPieceId pieceId) const;

		bool CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const;
		bool CanPromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const;
		bool IsChessPieceOnBoard(ChessPieceId chessPieceId) const;

		void MoveChessPieceToPosition(ChessPieceId chessPiece, const TilePosition& position);
		void PromotePawn(ChessPieceId pawnId, EChessPieceType promotionType);

	private:
		using ChessPicesPositions = std::unordered_map<ChessPieceId, TilePosition, ChessPieceIdHash>;

	private:
		ChessPicesPositions GetInitialChessPiecesPositions() const;
		void RefreshBoardState(const ChessPicesPositions& chessPiecesPositions);
		void RemoveChessPieceOnPosition(const TilePosition& position);

	private:
		ChessPicesPositions m_piecesPositions;
		std::vector<ChessPieceId> m_cachedBoardState;
		EnPassantCache m_enPassantCache;
	};
}

