#pragma once
#include "Utils/Utils.h"
#include "Utils/EnPassantCache.h"
#include "Utils/PlayerCastleCache.h"
#include "Utils/PlayerPossibleMoves.h"
#include "Utils/PlayerCheckStateResolver.h"

namespace chess
{
	class ChessPieceMovementIterator;

	class BoardService
	{
	public:
		BoardService();

		const std::vector<ChessPieceId>& GetBoardState() const;
		const std::vector<TilePosition>& GetChessPiecePossibleMoves(ChessPieceId pieceId) const;
		PlayerCheckStateResolver CreatePlayerCheckStateResolver(EColor playerColor) const;

		bool CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const;
		bool CanPromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const;
		bool IsChessPieceOnBoard(ChessPieceId chessPieceId) const;

		void MoveChessPieceToPosition(ChessPieceId chessPiece, const TilePosition& position);
		void PromotePawn(ChessPieceId pawnId, EChessPieceType promotionType);

	private:
		using ChessPicesPositions = std::unordered_map<ChessPieceId, TilePosition, ChessPieceIdHash>;

	private:
		ChessPicesPositions GetInitialChessPiecesPositions() const;
		void OnBoardChanged(const ChessPicesPositions& newBoard);

		EDirection GetCastleDirection(ChessPieceId movedPieceId, const TilePosition& movedToPosition) const;
		void OnCastle(EColor playerColor, EDirection castleDirection);

		TilePosition GetRemovalPosition(EChessPieceType movedPieceType, const TilePosition& movedToPosition) const;
		void RemoveChessPieceOnPosition(const TilePosition& position);
		void RefreshBoardState(const ChessPicesPositions& chessPiecesPositions);
		void RecalculatePossibleMoves(const ChessPicesPositions& chessPiecesPositions);

		PlayerCheckStateResolver::KingHitters GetKingHitters(EColor kingColor) const;
		std::unique_ptr<ChessPieceMovementIterator> CreatePossibleMovesIterator(ChessPieceId pieceId) const;

	private:
		ChessPicesPositions m_piecesPositions;
		std::vector<ChessPieceId> m_cachedBoardState;
		VectorMap<EColor, PlayerPossibleMoves> m_playerPossibleMoves;
		VectorMap<EColor, PlayerCastleCache> m_playerCastleCache;
		EnPassantCache m_enPassantCache;
	};
}

