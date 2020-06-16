#include "stdafx.h"
#include "PlayerCheckStateResolver.h"
#include "ChessPieces/ChessPieceTypes.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerCheckStateResolver::PlayerCheckStateResolver()
		: m_kingPosition(TilePosition::Invalid())
		, m_resolvedState(EPlayerCheckState::Clear)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerCheckStateResolver::PlayerCheckStateResolver(KingHitters&& kingHitters, const TilePosition& kingPosition, TilePositionSet playerReachablePositions)
		: m_kingHitters(std::move(kingHitters))
		, m_kingPosition(kingPosition)
		, m_reachablePositions(std::move(playerReachablePositions))
		, m_resolvedState(EPlayerCheckState::Clear)
	{
		m_resolvedState = ResolvePlayerCheckState();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EPlayerCheckState PlayerCheckStateResolver::GetCheckState() const
	{
		return m_resolvedState;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const PlayerCheckStateResolver::ShelterPositions& PlayerCheckStateResolver::GetShelterPositions() const
	{
		return m_shelterPositions;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EPlayerCheckState PlayerCheckStateResolver::ResolvePlayerCheckState()
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

		m_shelterPositions = FindSheltersForPositions(hitter.second);
		if (!m_shelterPositions.empty())
		{
			return EPlayerCheckState::Check;
		}

		return EPlayerCheckState::Checkmate;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerCheckStateResolver::ShelterPositions PlayerCheckStateResolver::FindSheltersForPositions(const TilePosition& hitterTilePosition) const
	{
		ShelterPositions result;

		const auto kingPosition = m_kingPosition.AsPosition();
		const auto hitterPosition = hitterTilePosition.AsPosition();

		const auto deltaTiles = kingPosition - hitterPosition;
		assert(deltaTiles.col == 0 || deltaTiles.row == 0 || std::abs(deltaTiles.col) == std::abs(deltaTiles.row));

		const int deltaFactor = std::max(deltaTiles.col, deltaTiles.row);
		const auto shelterCheckDirection = deltaTiles / deltaFactor;

		for (Position shelterCheckPosition = hitterPosition + shelterCheckDirection; shelterCheckPosition != kingPosition; shelterCheckPosition += shelterCheckDirection)
		{
			const bool isReachablePosition = m_reachablePositions.find(shelterCheckPosition) != m_reachablePositions.end();
			if (isReachablePosition)
			{
				result.emplace(shelterCheckPosition);
			}
		}

		return result;
	}
}