#include "Bishop.h"
#include "../Utils/Utils.h"

namespace chess
{
	Bishop::Bishop(EColor color)
		:ChessPiece(EChessPieceType::BishopType, color)
	{
		m_chessMoves.emplace_back(EDirection::ForwardLeft, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::ForwardRight, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::BackwardLeft, EMoveType::Multiple);
		m_chessMoves.emplace_back(EDirection::BackwardRight, EMoveType::Multiple);
	}
}