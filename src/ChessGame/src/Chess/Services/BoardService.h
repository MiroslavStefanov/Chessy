#pragma once
#include "Utils/Utils.h"
#include "Utils/EnPassantCache.h"
#include "Utils/PlayerCastleCache.h"
#include "Utils/PlayerPossibleMoves.h"
#include "Utils/PlayerCheckStateResolver.h"

#include "ErrorHandling/Exceptions/InvalidPlayerColorException.h"

namespace chess
{
	class ChessPieceMovementIterator;

	class BoardService
	{
	public:
		BoardService();

		const std::vector<ChessPieceId>& GetBoardState() const;
		std::vector<TilePosition> GetChessPiecePossibleMoves(ChessPieceId pieceId, bool isPlayerInCheck) const;
		EPlayerCheckState GetPlayerCheckState(EColor playerColor) const;

		bool CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position, bool isPlayerInCheck) const;
		bool CanPromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const;
		bool IsChessPieceOnBoard(ChessPieceId chessPieceId) const;

		void MoveChessPieceToPosition(ChessPieceId chessPiece, const TilePosition& position);
		void PromotePawn(ChessPieceId pawnId, EChessPieceType promotionType);

	private:
		using ChessPicesPositions = std::unordered_map<ChessPieceId, TilePosition, ChessPieceIdHash>;
		using PlayerProperties = VectorMap <EColor, std::tuple <PlayerPossibleMoves, PlayerCastleCache, PlayerCheckStateResolver>>;

	private:
		ChessPicesPositions GetInitialChessPiecesPositions() const;
		void OnBoardChanged(const ChessPicesPositions& newBoard, EColor currentTurnColor);

		template<class Property>
		Property& GetPlayerProperty(EColor playerColor);

		template<class Property>
		const Property& GetPlayerProperty(EColor playerColor) const;

		EDirection GetCastleDirection(ChessPieceId movedPieceId, const TilePosition& movedToPosition) const;
		void OnCastle(EColor playerColor, EDirection castleDirection);

		TilePosition GetRemovalPosition(EChessPieceType movedPieceType, const TilePosition& movedToPosition) const;
		void RemoveChessPieceOnPosition(const TilePosition& position);
		void RefreshBoardState(const ChessPicesPositions& chessPiecesPositions);
		void RecalculatePossibleMoves(const ChessPicesPositions& chessPiecesPositions, EColor currentTurnColor);
		PlayerCheckStateResolver CreatePlayerCheckStateResolver(EColor playerColor) const;

		std::unique_ptr<ChessPieceMovementIterator> CreatePossibleMovesIterator(ChessPieceId pieceId) const;
		void CalculateInitialPossibleMoves(const ChessPicesPositions& chessPiecesPositions);
		void RecalculatePossibleMovesForPlayer(const ChessPicesPositions& chessPiecesPositions, EColor playerColor);
		PlayerCheckStateResolver::KingHitters GetKingHitters(EColor kingColor) const;

	private:
		ChessPicesPositions m_piecesPositions;
		std::vector<ChessPieceId> m_cachedBoardState;

		PlayerProperties m_playerProperties;
		EnPassantCache m_enPassantCache;
	};
	template<class Property>
	inline Property& BoardService::GetPlayerProperty(EColor playerColor)
	{
		try
		{
			auto& properties = m_playerProperties[playerColor];
			return std::get<Property>(properties);
		}
		catch (std::exception & e)
		{
			throw InvalidPlayerColorException(e);
		}
	}
	template<class Property>
	inline const Property& BoardService::GetPlayerProperty(EColor playerColor) const
	{
		return const_cast<BoardService*>(this)->GetPlayerProperty<Property>(playerColor);
	}
}

