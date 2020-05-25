#include "stdafx.h"
#include "PlayerCastleCache.h"
#include "ChessPieces/ChessPieceTypes.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	const VectorMap<PlayerCastleCache::CastlePositionsKey, PlayerCastleCache::CastlePositions> PlayerCastleCache::CASTLE_POSITIONS_TABLE =
	{
		{
			CastlePositionsKey{EColor::White, EDirection::Left},
			CastlePositions{
				TilePosition(Position(CHESS_BOARD_SIDE - 1, 1)), //KingPosition
				TilePosition(Position(CHESS_BOARD_SIDE - 1, 2)), //RookPosition
				0 //RookInstance
			}
		},
		{
			CastlePositionsKey{EColor::White, EDirection::Right},
			CastlePositions{
				TilePosition(Position(CHESS_BOARD_SIDE - 1, 6)), //KingPosition
				TilePosition(Position(CHESS_BOARD_SIDE - 1, 5)), //RookPosition
				1 //RookInstance
			}
		},

		{
			CastlePositionsKey{EColor::Black, EDirection::Right},
			CastlePositions{
				TilePosition(Position(0, 1)), //KingPosition
				TilePosition(Position(0, 2)), //RookPosition
				0 //RookInstance
			}
		},
		{
			CastlePositionsKey{EColor::Black, EDirection::Left},
			CastlePositions{
				TilePosition(Position(0, 6)), //KingPosition
				TilePosition(Position(0, 5)), //RookPosition
				1 //RookInstance
			}
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerCastleCache::PlayerCastleCache(EColor playerColor) 
		: m_playerColor(playerColor)
		, m_hasMoved{
			{GetKingId(), false}, 
			{GetRookIdForDirection(EDirection::Left), false}, 
			{GetRookIdForDirection(EDirection::Right), false} 
		}
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerCastleCache::UpdateOnChessPieceMove(ChessPieceId chessPieceId)
	{
		if (m_hasMoved.HasKey(chessPieceId))
		{
			m_hasMoved[chessPieceId] = true;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool PlayerCastleCache::CanDoCastle(EDirection direction) const
	{
		return !m_hasMoved[GetKingId()] && !m_hasMoved[GetRookIdForDirection(direction)];
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	TilePosition PlayerCastleCache::GetKingPositionForCastle(EDirection castleDirection) const
	{
		return CASTLE_POSITIONS_TABLE[CastlePositionsKey{ m_playerColor, castleDirection }].KingPosition;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	TilePosition PlayerCastleCache::GetRookPositionForCastle(EDirection castleDirection) const
	{
		return CASTLE_POSITIONS_TABLE[CastlePositionsKey{ m_playerColor, castleDirection }].RookPosition;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceId PlayerCastleCache::GetKingId() const
	{
		return ChessPieceId(EChessPieceType::King, m_playerColor, 0);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceId PlayerCastleCache::GetRookIdForDirection(EDirection direction) const
	{
		const int rookInstance = CASTLE_POSITIONS_TABLE[CastlePositionsKey{ m_playerColor, direction }].RookInstanceNumber;
		return ChessPieceId(EChessPieceType::Rook, m_playerColor, rookInstance);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool PlayerCastleCache::CastlePositionsKey::operator==(const CastlePositionsKey& other) const
	{
		return Color == other.Color && Direction == other.Direction;
	}

}