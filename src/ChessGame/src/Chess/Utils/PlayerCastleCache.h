#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class PlayerCastleCache
	{
	private:
		struct CastlePositionsKey
		{
			EColor Color;
			EDirection Direction;

			bool operator ==(const CastlePositionsKey& other) const;
		};

		struct CastlePositions
		{
			TilePosition KingPosition;
			TilePosition RookPosition;
			int RookInstanceNumber;
		};

		static const VectorMap<CastlePositionsKey, CastlePositions> CASTLE_POSITIONS_TABLE;

	public:
		PlayerCastleCache(EColor playerColor);

		void UpdateOnChessPieceMove(ChessPieceId chessPieceId);
		bool CanDoCastle(EDirection direction) const;

		TilePosition GetKingPositionForCastle(EDirection castleDirection) const;
		TilePosition GetRookPositionForCastle(EDirection castleDirection) const;

		ChessPieceId GetKingId() const;
		ChessPieceId GetRookIdForDirection(EDirection direction) const;

	private:
		const EColor m_playerColor;
		VectorMap<ChessPieceId, bool> m_hasMoved;
	};
}

