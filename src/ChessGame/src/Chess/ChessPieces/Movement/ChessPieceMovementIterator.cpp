#include "stdafx.h"
#include "ChessPieceMovementIterator.h"
#include "ChessPieces/ChessPieceRegistry.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	const std::unordered_map<EDirection, Position> ChessPieceMovementIterator::STRAIGHT_MOVE_OFFSET_TABLE =
	{
		{ EDirection::ForwardLeft,		Position(1, -1) },
		{ EDirection::Forward,			Position(1, 0) },
		{ EDirection::ForwardRight,		Position(1, 1) },
		{ EDirection::Right,			Position(0, 1) },
		{ EDirection::BackwardRight,	Position(-1, 1) },
		{ EDirection::Backward,			Position(-1, 0) },
		{ EDirection::BackwardLeft,		Position(-1, -1) },
		{ EDirection::Left,				Position(0, -1) }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	const std::unordered_map<EDirection, Position> ChessPieceMovementIterator::KNIGHT_MOVE_OFFSET_TABLE =
	{
		{ EDirection::ForwardLeft,		Position(2, -1) },
		{ EDirection::Forward,			Position(2, 1) },
		{ EDirection::ForwardRight,		Position(1, 2) },
		{ EDirection::Right,			Position(-1, 2) },
		{ EDirection::BackwardRight,	Position(-2, 1) },
		{ EDirection::Backward,			Position(-2, -1) },
		{ EDirection::BackwardLeft,		Position(-1, -2) },
		{ EDirection::Left,				Position(1, -2) }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceMovementIterator::ChessPieceMovementIterator(const std::vector<ChessPieceId>& boardState, const TilePosition& initialPosition)
		: m_boardState(boardState)
		, m_initialPosition(initialPosition)
		, m_chessPieceId(ChessPieceId::Invalid())
		, m_gameMovesIndex(0)
		, m_currentPosition(TilePosition::Invalid())
		, m_nextStartingPosition(initialPosition)
		, m_successfulMove(false)
	{
		assert(initialPosition.IsValid());
		if (initialPosition.IsValid())
		{
			m_chessPieceId = boardState[initialPosition.AsIndex()];
		}

		assert(m_chessPieceId.IsValid());
		Advance();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessPieceMovementIterator::Advance()
	{
		m_successfulMove = false;
		while (!m_successfulMove && !IsDone())
		{
			const auto& gameMove = GetChessPieceMove();
			if (gameMove.IsValid())
			{
				TryMove(gameMove);
			}
			else
			{
				Finish();
			}
		} 
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessPieceMovementIterator::IsDone() const
	{
		const auto& gameMoves = ChessPieceRegistry::GetGameMoves(m_chessPieceId.GetType());
		return m_gameMovesIndex > gameMoves.size();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& ChessPieceMovementIterator::GetTilePosition() const
	{
		return m_currentPosition;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessPieceMovementIterator::TryMove(const ChessPieceMove& move)
	{
		const TilePosition nextPositionCandidate = CalculateMovePosition(move);

		bool continueGameMove = false;
		if (nextPositionCandidate.IsValid())
		{
			const auto& nextPositionChessPiece = m_boardState[nextPositionCandidate.AsIndex()];
			const bool isPositionCandidateEmpty = !nextPositionChessPiece.IsValid();
			if (isPositionCandidateEmpty || nextPositionChessPiece.GetColor() != m_chessPieceId.GetColor())
			{
				SetTilePosition(nextPositionCandidate);
				continueGameMove = isPositionCandidateEmpty && move.GetMoveType() == EMoveType::Multiple;
			}
		}

		if (!continueGameMove)
		{
			StartNextGameMove();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const ChessPieceMove& ChessPieceMovementIterator::GetChessPieceMove() const
	{
		const auto& gameMoves = ChessPieceRegistry::GetGameMoves(m_chessPieceId.GetType());
		if (m_gameMovesIndex < gameMoves.size())
		{
			return gameMoves.at(m_gameMovesIndex);
		}

		return ChessPieceMove::Invalid();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	TilePosition ChessPieceMovementIterator::CalculateMovePosition(const ChessPieceMove& move) const
	{
		return TilePosition(m_nextStartingPosition.AsPosition() + GetMoveOffset(move));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	Position ChessPieceMovementIterator::GetMoveOffset(const ChessPieceMove& move) const
	{
		const auto& movesTable = move.GetMoveType() == EMoveType::KnightMove ? KNIGHT_MOVE_OFFSET_TABLE : STRAIGHT_MOVE_OFFSET_TABLE;
		Position offset = movesTable.at(move.GetDirection());

		if (move.GetMoveType() == EMoveType::PawnJump)
		{
			offset *= 2;
		}
		if (m_chessPieceId.GetColor() == EColor::White)
		{
			offset *= -1;
		}

		return offset;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessPieceMovementIterator::Finish()
	{
		m_gameMovesIndex = ChessPieceRegistry::GetGameMoves(m_chessPieceId.GetType()).size() + 1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessPieceMovementIterator::StartNextGameMove()
	{
		++m_gameMovesIndex;
		m_nextStartingPosition = m_initialPosition;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessPieceMovementIterator::SetTilePosition(const TilePosition& position)
	{
		m_currentPosition = position;
		m_nextStartingPosition = position;
		m_successfulMove = true;
	}

	
}