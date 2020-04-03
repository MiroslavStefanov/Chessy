#include "King.h"
#include "../Utils/Utils.h"

namespace chess
{
	King::King(EColor color)
		:ChessPiece(EChessPieceType::KingType, color)
	{
		m_chessMoves.emplace_back(EDirection::ForwardLeft, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Forward, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::ForwardRight, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Right, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::BackwardRight, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Backward, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::BackwardLeft, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::Left, EMoveType::KingMove);
		m_chessMoves.emplace_back(EDirection::NoDirection, EMoveType::Castle);
	}
}
