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
	{
		m_piecesPositions = GetInitialChessPiecesPositions();
		RefreshBoardState(m_piecesPositions);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<ChessPieceId>& BoardService::GetBoardState() const
	{
		return m_cachedBoardState;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<ChessPieceMovementIterator> BoardService::CreatePossibleMovesIterator(ChessPieceId pieceId) const
	{
		assert(IsChessPieceOnBoard(pieceId));

		const auto& piecePosition = m_piecesPositions.at(pieceId);
		switch (pieceId.GetType())
		{
		case EChessPieceType::Pawn:
			return std::make_unique<PawnMovementIterator>(m_cachedBoardState, piecePosition, m_enPassantCache.GetEnPassantPosition());
		case EChessPieceType::King:
			return std::make_unique<KingMovementIterator>(m_cachedBoardState, piecePosition);
		default:
			return std::make_unique<ChessPieceMovementIterator>(m_cachedBoardState, piecePosition);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool BoardService::CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const
	{
		auto possibleMovesIterator = CreatePossibleMovesIterator(chessPieceId);
		assert(possibleMovesIterator);

		while (!possibleMovesIterator->IsDone())
		{
			if (possibleMovesIterator->GetTilePosition() == position)
			{
				return true;
			}

			possibleMovesIterator->Advance();
		}

		return false;
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
		assert(chessPiece.IsValid() && position.IsValid() && IsChessPieceOnBoard(chessPiece));

		const bool isEnPassant = m_enPassantCache.IsValid() 
			&& m_enPassantCache.GetEnPassantPosition() == position 
			&& chessPiece.GetType() == EChessPieceType::Pawn;

		RemoveChessPieceOnPosition(isEnPassant ? m_enPassantCache.GetPawnPosition() : position);
		auto oldPosition = m_piecesPositions.at(chessPiece);
		m_piecesPositions.at(chessPiece) = position;

		RefreshBoardState(m_piecesPositions);
		m_enPassantCache.UpdateOnChessPieceMove(chessPiece, oldPosition, position);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::PromotePawn(ChessPieceId pawnId, EChessPieceType promotionType)
	{
		assert(pawnId.GetType() == EChessPieceType::Pawn && promotionType != EChessPieceType::Pawn && promotionType != EChessPieceType::COUNT);

		auto pawnPosition = m_piecesPositions.at(pawnId);
		m_piecesPositions.erase(pawnId);
		
		ChessPieceId promotedPieceId = ChessPieceId(
			promotionType,
			pawnId.GetColor(),
			ChessPieceRegistry::GetInstancesCount(promotionType) + pawnId.GetInstanceNumber()
		);
		m_piecesPositions.emplace(promotedPieceId, pawnPosition);

		RefreshBoardState(m_piecesPositions);
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
	void BoardService::RemoveChessPieceOnPosition(const TilePosition& position)
	{
		auto pieceToRemove = m_cachedBoardState[position.AsIndex()];
		if (pieceToRemove.IsValid())
		{
			m_piecesPositions.at(pieceToRemove) = TilePosition::Invalid();
		}
	}
}