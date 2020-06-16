#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class PlayerCheckStateResolver
	{
	public:
		using KingHitters = std::vector<std::pair<ChessPieceId, TilePosition>>;
		using ShelterPositions = std::unordered_set<TilePosition, TilePositionHash>;

	public:
		PlayerCheckStateResolver();
		PlayerCheckStateResolver(KingHitters&& kingHitters, const TilePosition& kingPosition, TilePositionSet playerReachablePositions);
		
		EPlayerCheckState GetCheckState() const;
		const ShelterPositions& GetShelterPositions() const;

	private:
		EPlayerCheckState ResolvePlayerCheckState();
		ShelterPositions FindSheltersForPositions(const TilePosition& hitterTilePosition) const;

	private:
		KingHitters m_kingHitters;
		TilePosition m_kingPosition;
		TilePositionSet m_reachablePositions;

		EPlayerCheckState m_resolvedState;
		ShelterPositions m_shelterPositions;
	};
}

