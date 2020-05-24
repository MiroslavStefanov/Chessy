#include "stdafx.h"
#include "EnPassantCache.h"
#include "Utils/PawnJumpValidator.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	bool EnPassantCache::IsValid() const
	{
		return m_pawnPosition.IsValid() && m_enPassantPosition.IsValid();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& EnPassantCache::GetEnPassantPosition() const
	{
		return m_enPassantPosition;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& EnPassantCache::GetPawnPosition() const
	{
		return m_pawnPosition;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void EnPassantCache::UpdateOnChessPieceMove(ChessPieceId chessPieceId, const TilePosition& oldPosition, const TilePosition& newPosition)
	{
		auto jumpValidator = PawnJumpValidator::CreateFromMovedChessPiece(chessPieceId, oldPosition, newPosition);
		if (jumpValidator.IsValidJump())
		{
			Set(jumpValidator.GetEnPassantPosition(), newPosition);
		}
		else
		{
			Reset();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void EnPassantCache::Set(const TilePosition& enPassantPosition, const TilePosition& pawnPosition)
	{
		m_enPassantPosition = enPassantPosition;
		m_pawnPosition = pawnPosition;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void EnPassantCache::Reset()
	{
		Set(TilePosition::Invalid(), TilePosition::Invalid());
	}
}