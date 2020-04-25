#include "stdafx.h"
#include "PawnJumpValidator.h"
#include "ChessPieces/ChessPieceRegistry.h"
#include "ChessPieces/ChessPieceTypes.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	PawnJumpValidator PawnJumpValidator::CreateFromMovedChessPiece(ChessPieceId chessPieceId, const TilePosition& startPosition, const TilePosition& endPosition)
	{
		PawnJumpValidator validator;

		validator.m_chessPieceId = chessPieceId;
		validator.m_startPosition = startPosition;
		validator.m_endPosition = endPosition;

		return validator;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	PawnJumpValidator::PawnJumpValidator()
		: m_chessPieceId(ChessPieceId::Invalid())
		, m_startPosition(TilePosition::Invalid())
		, m_endPosition(TilePosition::Invalid())
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////
	Position PawnJumpValidator::GetHalfJumpDelta() const
	{
		return m_chessPieceId.GetColor() == EColor::White ? Position(-1, 0) : Position(1, 0);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool PawnJumpValidator::IsValidJump() const
	{
		if (!m_chessPieceId.IsValid() || m_chessPieceId.GetType() != EChessPieceType::Pawn)
		{
			return false;
		}

		if (!m_startPosition.IsValid() || !m_endPosition.IsValid())
		{
			return false;
		}

		const auto& initialPawnPosition = ChessPieceRegistry::GetInitialPosition(
			m_chessPieceId.GetType(),
			m_chessPieceId.GetColor(),
			m_chessPieceId.GetInstanceNumber()
		);

		if (m_startPosition != initialPawnPosition)
		{
			return false;
		}

		const Position jumpDelta = GetHalfJumpDelta() * 2;
		const Position moveDelta = m_endPosition.AsPosition() - m_startPosition.AsPosition();

		return moveDelta == jumpDelta;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	TilePosition PawnJumpValidator::GetEnPassantPosition() const
	{
		return TilePosition(m_startPosition.AsPosition() + GetHalfJumpDelta());
	}

}