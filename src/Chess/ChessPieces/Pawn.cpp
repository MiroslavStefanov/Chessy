#include "Pawn.h"
#include "../Utils/Utils.h"

namespace chess
{
	Pawn::Pawn(EColor color)
		:ChessPiece(EChessPieceType::PawnType, color)
	{
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::PawnMove);
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::PawnJump);
		m_chessMoves.emplace_back(EDirection::ForwardLeft, EMoveType::PawnHit);
		m_chessMoves.emplace_back(EDirection::ForwardRight, EMoveType::PawnHit);
	}
}