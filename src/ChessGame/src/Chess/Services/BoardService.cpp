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
		, m_playerCheckStateResolvers{
			{EColor::White, PlayerCheckStateResolver()},
			{EColor::Black, PlayerCheckStateResolver()}
		}
	{
		m_piecesPositions = GetInitialChessPiecesPositions();
		OnBoardChanged(m_piecesPositions, EColor::Colorless);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<ChessPieceId>& BoardService::GetBoardState() const
	{
		return m_cachedBoardState;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::vector<TilePosition> BoardService::GetChessPiecePossibleMoves(ChessPieceId pieceId, bool isPlayerInCheck) const
	{
		assert(pieceId.IsValid());

		auto possibleMoves = m_playerPossibleMoves[pieceId.GetColor()].GetChessPiecePossibleMoves(pieceId);
		if (isPlayerInCheck)
		{
			const auto& shelterPositions = m_playerCheckStateResolvers[pieceId.GetColor()].GetShelterPositions();
			auto newEndIt = std::remove_if(possibleMoves.begin(), possibleMoves.end(), [&shelterPositions](const TilePosition& position)
				{
					return shelterPositions.find(position) == shelterPositions.end();
				}
			);
			possibleMoves.erase(newEndIt, possibleMoves.end());
		}

		return possibleMoves;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EPlayerCheckState BoardService::GetPlayerCheckState(EColor playerColor) const
	{
		assert(m_playerCheckStateResolvers.HasKey(playerColor));
		return m_playerCheckStateResolvers[playerColor].GetCheckState();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerCheckStateResolver BoardService::CreatePlayerCheckStateResolver(EColor playerColor) const
	{
		const auto kingId = m_playerCastleCache[playerColor].GetKingId();
		const auto& kingPosition = m_piecesPositions.at(kingId);
		const auto& possibleMoves = m_playerPossibleMoves[playerColor];
		return PlayerCheckStateResolver(GetKingHitters(playerColor), kingPosition, possibleMoves.GetPlayerPossibleMoves());
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool BoardService::CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position, bool isPlayerInCheck) const
	{
		if (!chessPieceId.IsValid())
		{
			return false;
		}

		const auto& possibleMoves = GetChessPiecePossibleMoves(chessPieceId, isPlayerInCheck);
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

		OnBoardChanged(m_piecesPositions, chessPiece.GetColor());
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

		OnBoardChanged(m_piecesPositions, pawnId.GetColor());
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
	void BoardService::OnBoardChanged(const ChessPicesPositions& newBoard, EColor currentTurnColor)
	{
		RefreshBoardState(newBoard);
		RecalculatePossibleMoves(newBoard);

		const auto enemyColor = GetAlternateColor(currentTurnColor);
		if (m_playerCheckStateResolvers.HasKey(enemyColor))
		{
			m_playerCheckStateResolvers[enemyColor] = CreatePlayerCheckStateResolver(enemyColor);
		}
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
		
		m_playerPossibleMoves[EColor::White].InvalidatePlayerPossibleMovesCache();
		m_playerPossibleMoves[EColor::Black].InvalidatePlayerPossibleMovesCache();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerCheckStateResolver::KingHitters BoardService::GetKingHitters(EColor kingColor) const
	{
		const auto kingId = m_playerCastleCache[kingColor].GetKingId();
		assert(IsChessPieceOnBoard(kingId));

		const auto& kingPosition = m_piecesPositions.at(kingId);
		const auto enemyPlayerColor = GetAlternateColor(kingColor);

		const auto& kingHittersIds = m_playerPossibleMoves[enemyPlayerColor].GetTilePositionHitters(kingPosition);
		PlayerCheckStateResolver::KingHitters kingHitters;
		std::transform(kingHittersIds.cbegin(), kingHittersIds.cend(), std::back_inserter(kingHitters), [this](ChessPieceId hitterId)
			{
				return std::make_pair(hitterId, m_piecesPositions.at(hitterId));
			}
		);

		return kingHitters;
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