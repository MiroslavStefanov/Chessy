#include "stdafx.h"
#include "KingMovementIterator.h"
#include "ChessPieces/ChessPieceTypes.h"
#include "ChessPieces/ChessPieceRegistry.h"
#include "Utils/PlayerCastleCache.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	KingMovementIterator::KingMovementIterator(const std::vector<ChessPieceId>& boardState, const TilePosition& initialPosition, TilePositionSet&& checkPositions, const PlayerCastleCache& castleCache)
		: ChessPieceMovementIterator(boardState, initialPosition)
		, m_checkPositions(std::move(checkPositions))
		, m_castleCache(castleCache)
	{
		assert(m_chessPieceId.GetType() == EChessPieceType::King);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void KingMovementIterator::TryMove(const ChessPieceMove& move)
	{
		if (move.GetMoveType() == EMoveType::Castle)
		{
			const auto direction = move.GetDirection();
			assert(IsValidCastleDirection(direction));

			TryCastle(direction);
			return;
		}

		const TilePosition nextPositionCandidate = CalculateMovePosition(move);
		if (!nextPositionCandidate.IsValid() || m_boardState[nextPositionCandidate.AsIndex()].GetColor() == m_chessPieceId.GetColor())
		{
			StartNextGameMove();
			return;
		}

		if (m_checkPositions.find(nextPositionCandidate) != m_checkPositions.end())
		{
			StartNextGameMove();
			return;
		}

		SetTilePosition(nextPositionCandidate);
		StartNextGameMove();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void KingMovementIterator::TryCastle(EDirection direction)
	{
		if (!m_castleCache.CanDoCastle(direction))
		{
			StartNextGameMove();
			return;
		}

		const auto kingToRookDirection = m_castleCache.GetKingPositionForCastle(direction).AsPosition() - m_castleCache.GetRookPositionForCastle(direction).AsPosition();
		const auto rookId = m_castleCache.GetRookIdForDirection(direction);
		const auto rookInitialPosition = ChessPieceRegistry::GetInitialPosition(rookId.GetType(), rookId.GetColor(), rookId.GetInstanceNumber()).AsPosition();
		for (Position intermediatePosition = m_initialPosition.AsPosition() + kingToRookDirection; intermediatePosition != rookInitialPosition; intermediatePosition += kingToRookDirection)
		{
			const TilePosition intermediateTile = TilePosition(intermediatePosition);
			assert(intermediateTile.IsValid());

			const bool hasPiece = m_boardState[intermediateTile.AsIndex()].IsValid();
			const bool isInCheck = m_checkPositions.find(intermediateTile) != m_checkPositions.end();
			if (hasPiece || isInCheck)
			{
				StartNextGameMove();
				return;
			}
		}

		SetTilePosition(m_castleCache.GetKingPositionForCastle(direction));
		StartNextGameMove();
	}
}