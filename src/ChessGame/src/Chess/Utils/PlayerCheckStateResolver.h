#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class PlayerCheckStateResolver
	{
	public:
		using KingHitters = std::vector<std::pair<ChessPieceId, TilePosition>>;
	public:
		PlayerCheckStateResolver(KingHitters&& kingHitters, const TilePosition& kingPosition, TilePositionSet playerReachablePositions);
		EPlayerCheckState ResolvePlayerCheckState() const;

	private:
		bool CanFindShelter(const TilePosition& hitterTilePosition) const;

	private:
		KingHitters m_kingHitters;
		TilePosition m_kingPosition;
		TilePositionSet m_reachablePositions;
	};
}

