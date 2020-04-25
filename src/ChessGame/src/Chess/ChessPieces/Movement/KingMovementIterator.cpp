#include "stdafx.h"
#include "KingMovementIterator.h"
#include "ChessPieces/ChessPieceTypes.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	KingMovementIterator::KingMovementIterator(const std::vector<ChessPieceId>& boardState, const TilePosition& initialPosition)
		: ChessPieceMovementIterator(boardState, initialPosition)
	{
		assert(m_chessPieceId.GetType() == EChessPieceType::King);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void KingMovementIterator::TryMove(const ChessPieceMove& move)
	{
		const TilePosition nextPositionCandidate = CalculateMovePosition(move);

		if (!nextPositionCandidate.IsValid() || m_boardState[nextPositionCandidate.AsIndex()].GetColor() == m_chessPieceId.GetColor())
		{
			StartNextGameMove();
			return;
		}

		//TODO: implement castle and check
		SetTilePosition(nextPositionCandidate);
		StartNextGameMove();
	}
}