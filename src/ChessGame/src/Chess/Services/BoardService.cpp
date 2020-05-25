#include "stdafx.h"
#include "BoardService.h"
#include "ChessPieces/ChessPieceRegistry.h"
#include "ChessPieces/ChessPiece.h"
#include "ChessPieces/Movement/ChessPieceMovementIterator.h"
#include "ChessPieces/Movement/PawnMovementIterator.h"
#include "ChessPieces/Movement/KingMovementIterator.h"
#include "ChessPieces/ChessPieceTypes.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	BoardService::BoardService() 
		: m_playerPossibleMoves{ 
			{EColor::White, PlayerPossibleMoves()}, 
			{EColor::Black, PlayerPossibleMoves()}
		}
		, m_playerCastleCache{ 
			{EColor::White, PlayerCastleCache(EColor::White)}, 
			{EColor::Black, PlayerCastleCache(EColor::Black)} 
		}
	{
		m_piecesPositions = GetInitialChessPiecesPositions();
		OnBoardChanged(m_piecesPositions);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<ChessPieceId>& BoardService::GetBoardState() const
	{
		return m_cachedBoardState;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<TilePosition>& BoardService::GetChessPiecePossibleMoves(ChessPieceId pieceId) const
	{
		assert(pieceId.IsValid());
		return m_playerPossibleMoves[pieceId.GetColor()].GetChessPiecePossibleMoves(pieceId);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool BoardService::CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const
	{
		if (!chessPieceId.IsValid())
		{
			return false;
		}

		const auto& possibleMoves = GetChessPiecePossibleMoves(chessPieceId);
		return std::find(possibleMoves.cbegin(), possibleMoves.cend(), position) != possibleMoves.cend();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool BoardService::CanPromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const
	{
		if (!pawnId.IsValid() || pawnId.GetType() != EChessPieceType::Pawn)
		{
			return false;
		}

		auto it = m_piecesPositions.find(pawnId);
		if (it == m_piecesPositions.end())
		{
			return false;
		}

		return IsPawnPromotionPosition(pawnId.GetColor(), it->second);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool BoardService::IsChessPieceOnBoard(ChessPieceId chessPieceId) const
	{
		auto it = m_piecesPositions.find(chessPieceId);
		return it != m_piecesPositions.end() && it->second.IsValid();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::MoveChessPieceToPosition(ChessPieceId chessPiece, const TilePosition& position)
	{
		assert(CanMoveChessPieceToPosition(chessPiece, position));

		const auto castleDirection = GetCastleDirection(chessPiece, position);
		if (IsValidCastleDirection(castleDirection))
		{
			OnCastle(chessPiece.GetColor(), castleDirection);
		}

		const auto removalPosition = GetRemovalPosition(chessPiece.GetType(), position);
		RemoveChessPieceOnPosition(removalPosition);

		auto oldPosition = m_piecesPositions.at(chessPiece);
		m_piecesPositions.at(chessPiece) = position;

		m_enPassantCache.UpdateOnChessPieceMove(chessPiece, oldPosition, position);
		m_playerCastleCache[chessPiece.GetColor()].UpdateOnChessPieceMove(chessPiece);

		OnBoardChanged(m_piecesPositions);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::PromotePawn(ChessPieceId pawnId, EChessPieceType promotionType)
	{
		assert(CanPromotePawn(pawnId, promotionType));

		ChessPieceId promotedPieceId = ChessPieceId(
			promotionType,
			pawnId.GetColor(),
			ChessPieceRegistry::GetInstancesCount(promotionType) + pawnId.GetInstanceNumber()
		);

		auto pawnPosition = m_piecesPositions.at(pawnId);
		m_piecesPositions.erase(pawnId);
		m_piecesPositions.emplace(promotedPieceId, pawnPosition);

		OnBoardChanged(m_piecesPositions);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	BoardService::ChessPicesPositions BoardService::GetInitialChessPiecesPositions() const
	{
		ChessPicesPositions result;
		for (int pieceType = 0; pieceType < (int)EChessPieceType::COUNT; ++pieceType)
		{
			const EChessPieceType type = (EChessPieceType)pieceType;
			const std::size_t instancesCount = ChessPieceRegistry::GetInstancesCount(type);
			for (int instace = 0; instace < instancesCount; ++instace)
			{
				result.emplace(ChessPieceId(type, EColor::White, instace), ChessPieceRegistry::GetInitialPosition(type, EColor::White, instace));
				result.emplace(ChessPieceId(type, EColor::Black, instace), ChessPieceRegistry::GetInitialPosition(type, EColor::Black, instace));
			}
		}

		return result;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::OnBoardChanged(const ChessPicesPositions& newBoard)
	{
		RefreshBoardState(newBoard);
		RecalculatePossibleMoves(newBoard);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EDirection BoardService::GetCastleDirection(ChessPieceId movedPieceId, const TilePosition& movedToPosition) const
	{
		auto& playerCastleCache = m_playerCastleCache[movedPieceId.GetColor()];
		if (movedPieceId != playerCastleCache.GetKingId())
		{
			return EDirection::COUNT;
		}

		for (auto direction : { EDirection::Left, EDirection::Right })
		{
			if (playerCastleCache.GetKingPositionForCastle(direction) == movedToPosition)
			{
				return direction;
			}
		}

		return EDirection::COUNT;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::OnCastle(EColor playerColor, EDirection castleDirection)
	{
		auto& castleCache = m_playerCastleCache[playerColor];
		auto castleRookId = castleCache.GetRookIdForDirection(castleDirection);
		m_piecesPositions.at(castleRookId) = castleCache.GetRookPositionForCastle(castleDirection);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	TilePosition BoardService::GetRemovalPosition(EChessPieceType movedPieceType, const TilePosition& movedToPosition) const
	{
		const bool isEnPassant = m_enPassantCache.IsValid()
			&& m_enPassantCache.GetEnPassantPosition() == movedToPosition
			&& movedPieceType == EChessPieceType::Pawn;

		return isEnPassant ? m_enPassantCache.GetPawnPosition() : movedToPosition;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::RemoveChessPieceOnPosition(const TilePosition& position)
	{
		assert(position.IsValid());
		auto pieceToRemove = m_cachedBoardState[position.AsIndex()];
		if (pieceToRemove.IsValid())
		{
			m_piecesPositions.at(pieceToRemove) = TilePosition::Invalid();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::RefreshBoardState(const ChessPicesPositions& chessPiecesPositions)
	{
		std::vector<ChessPieceId> emptyBoard(CHESS_BOARD_SIDE * CHESS_BOARD_SIDE, ChessPieceId::Invalid());
		m_cachedBoardState.swap(emptyBoard);

		for (const auto& [pieceId, tilePosition] : chessPiecesPositions)
		{
			if (tilePosition.IsValid())
			{
				m_cachedBoardState[tilePosition.AsIndex()] = pieceId;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::RecalculatePossibleMoves(const ChessPicesPositions& chessPiecesPositions)
	{
		for (const auto& [chessPieceId, chessPiecePosition] : chessPiecesPositions)
		{
			auto& playerMoves = m_playerPossibleMoves[chessPieceId.GetColor()];
			if (chessPiecePosition.IsValid())
			{
				playerMoves.UpdateChessPiecePossibleMoves(chessPieceId, CreatePossibleMovesIterator(chessPieceId));
			}
			else
			{
				playerMoves.RemoveChessPiecePossibleMoves(chessPieceId);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<ChessPieceMovementIterator> BoardService::CreatePossibleMovesIterator(ChessPieceId pieceId) const
	{
		assert(IsChessPieceOnBoard(pieceId));

		std::unique_ptr<ChessPieceMovementIterator> result;
		const auto& piecePosition = m_piecesPositions.at(pieceId);
		switch (pieceId.GetType())
		{
		case EChessPieceType::Pawn:
		{
			result = std::make_unique<PawnMovementIterator>(m_cachedBoardState, piecePosition, m_enPassantCache.GetEnPassantPosition());
		}
		break;
		case EChessPieceType::King:
		{
			const auto& enemyPossibleMoves = m_playerPossibleMoves[GetAlternateColor(pieceId.GetColor())];
			const auto& castleCache = m_playerCastleCache[pieceId.GetColor()];
			result = std::make_unique<KingMovementIterator>(m_cachedBoardState, piecePosition, enemyPossibleMoves.GetPlayerPossibleMoves(), castleCache);
		}
		break;
		default:
		{
			result = std::make_unique<ChessPieceMovementIterator>(m_cachedBoardState, piecePosition);
		}
		break;
		}

		assert(result);
		result->Advance();

		return result;
	}
}