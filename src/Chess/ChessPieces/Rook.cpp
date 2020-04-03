#include "Rook.h"
#include "../Utils/Utils.h"

namespace chess
{
	Rook::Rook(EColor color)
		:ChessPiece(EChessPieceType::RookType, color)
	{
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::Backward, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::Left, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::Right, EMoveType::Multiple);
	}
}