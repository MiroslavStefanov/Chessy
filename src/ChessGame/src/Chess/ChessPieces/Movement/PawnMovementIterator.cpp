#include "stdafx.h"
#include "PawnMovementIterator.h"
#include "ChessPieces/ChessPieceRegistry.h"
#include "ChessPieces/ChessPieceTypes.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	PawnMovementIterator::PawnMovementIterator(const std::vector<ChessPieceId>& boardState, const TilePosition& initialPosition, const TilePosition& enPassantPosition)
		: ChessPieceMovementIterator(boardState, initialPosition)
		, m_enPassantPosition(enPassantPosition)
	{
		assert(m_chessPieceId.GetType() == EChessPieceType::Pawn);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void PawnMovementIterator::TryMove(const ChessPieceMove& move)
	{
		const TilePosition nextPositionCandidate = CalculateMovePosition(move);

		bool validMove = false;
		if (nextPositionCandidate.IsValid())
		{
			switch (move.GetMoveType())
			{
			case EMoveType::PawnMove:
				validMove = CanDoPawnMove(nextPositionCandidate);
				break;

			case EMoveType::PawnJump:
				validMove = CanDoPawnJump(nextPositionCandidate);
				break;

			case EMoveType::PawnHit:
				validMove = CanDoPawnHit(nextPositionCandidate);
				break;

			default:
				assert(false);
				break;
			}
		}

		if (validMove)
		{
			SetTilePosition(nextPositionCandidate);
		}

		StartNextGameMove();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool PawnMovementIterator::CanDoPawnMove(const TilePosition& position) const
	{
		const auto& nextPositionChessPiece = m_boardState[position.AsIndex()];
		const bool isPositionEmpty = !nextPositionChessPiece.IsValid();

		return isPositionEmpty;

	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool PawnMovementIterator::CanDoPawnJump(const TilePosition& position) const
	{
		const auto& nextPositionChessPiece = m_boardState[position.AsIndex()];
		const bool isPositionEmpty = !nextPositionChessPiece.IsValid();

		const TilePosition& initialPawnPosition = ChessPieceRegistry::GetInitialPosition(
			m_chessPieceId.GetType(),
			m_chessPieceId.GetColor(),
			m_chessPieceId.GetInstanceNumber()
		);
		const bool canJump = m_initialPosition == initialPawnPosition;

		return isPositionEmpty && canJump;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool PawnMovementIterator::CanDoPawnHit(const TilePosition& position) const
	{
		const auto& nextPositionChessPiece = m_boardState[position.AsIndex()];

		const bool canHitEnemyPiece = nextPositionChessPiece.IsValid() && nextPositionChessPiece.GetColor() != m_chessPieceId.GetColor();
		const bool canDoEnPassant = position == m_enPassantPosition;

		return canHitEnemyPiece || canDoEnPassant;
	}
}