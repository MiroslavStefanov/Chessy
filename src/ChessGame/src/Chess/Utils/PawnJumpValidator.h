#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class PawnJumpValidator
	{
	public:
		static PawnJumpValidator CreateFromMovedChessPiece(ChessPieceId chessPieceId, const TilePosition& startPosition, const TilePosition& endPosition);

		bool IsValidJump() const;
		TilePosition GetEnPassantPosition() const;

	private:
		PawnJumpValidator();
		Position GetHalfJumpDelta() const;

	private:
		ChessPieceId m_chessPieceId;
		TilePosition m_startPosition;
		TilePosition m_endPosition;
	};
}

