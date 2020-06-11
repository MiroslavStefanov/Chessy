#include "stdafx.h"
#include "PlayerCheckStateResolver.h"
#include "ChessPieces/ChessPieceTypes.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerCheckStateResolver::PlayerCheckStateResolver(KingHitters&& kingHitters, const TilePosition& kingPosition, TilePositionSet playerReachablePositions)
		: m_kingHitters(std::move(kingHitters))
		, m_kingPosition(kingPosition)
		, m_reachablePositions(std::move(playerReachablePositions))
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EPlayerCheckState PlayerCheckStateResolver::ResolvePlayerCheckState() const
	{
		static constexpr std::size_t CHECKMATE_MIN_HITTERS_COUNT = 2;
		static constexpr EChessPieceType CHECKMATE_NO_SHELTER_PIECE_TYPE = EChessPieceType::Knight;

		if (m_kingHitters.empty())
		{
			return EPlayerCheckState::Clear;
		}

		if (m_kingHitters.size() >= CHECKMATE_MIN_HITTERS_COUNT)
		{
			return EPlayerCheckState::Checkmate;
		}

		//There is exactly one hitter
		const auto& hitter = m_kingHitters.front();

		const bool canTakeHitter = m_reachablePositions.find(hitter.second) != m_reachablePositions.end();
		if (canTakeHitter)
		{
			return EPlayerCheckState::Check;
		}

		if (hitter.first.GetType() == CHECKMATE_NO_SHELTER_PIECE_TYPE)
		{
			return EPlayerCheckState::Checkmate;
		}

		if (CanFindShelter(hitter.second))
		{
			return EPlayerCheckState::Check;
		}

		return EPlayerCheckState::Checkmate;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool PlayerCheckStateResolver::CanFindShelter(const TilePosition& hitterTilePosition) const
	{
		const auto kingPosition = m_kingPosition.AsPosition();
		const auto hitterPosition = hitterTilePosition.AsPosition();

		const auto deltaTiles = kingPosition - hitterPosition;
		assert(deltaTiles.col == 0 || deltaTiles.row == 0 || deltaTiles.col == deltaTiles.row);

		const int deltaFactor = std::max(deltaTiles.col, deltaTiles.row);
		const auto shelterCheckDirection = deltaTiles / deltaFactor;

		for (Position shelterCheckPosition = hitterPosition + shelterCheckDirection; shelterCheckPosition != kingPosition; shelterCheckPosition += shelterCheckDirection)
		{
			const bool isReachablePosition = m_reachablePositions.find(shelterCheckPosition) != m_reachablePositions.end();
			if (isReachablePosition)
			{
				return true;
			}
		}

		return false;
	}

}