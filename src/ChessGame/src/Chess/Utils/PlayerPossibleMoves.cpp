#include "stdafx.h"
#include "PlayerPossibleMoves.h"

#include "ChessPieces/Movement/ChessPieceMovementIterator.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerPossibleMoves::UpdateChessPiecePossibleMoves(ChessPieceId pieceId, std::unique_ptr<ChessPieceMovementIterator>&& pieceMovementIterator)
	{
		assert(pieceMovementIterator);

		auto& possibleMoves = m_possibleMoves[pieceId];
		possibleMoves.clear();

		while (!pieceMovementIterator->IsDone())
		{
			const auto& tilePosition = pieceMovementIterator->GetTilePosition();
			possibleMoves.push_back(tilePosition);
			m_positionHitters[tilePosition].push_back(pieceId);

			pieceMovementIterator->Advance();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerPossibleMoves::RemoveChessPiecePossibleMoves(ChessPieceId pieceId)
	{
		const auto& possibleMoves = m_possibleMoves.at(pieceId);

		for (const auto& position : possibleMoves)
		{
			RemoveHitterFromPosition(pieceId, position);
		}

		m_possibleMoves.erase(pieceId);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<TilePosition>& PlayerPossibleMoves::GetChessPiecePossibleMoves(ChessPieceId pieceId) const
	{
		return m_possibleMoves.at(pieceId);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<ChessPieceId>& PlayerPossibleMoves::GetTilePositionHitters(const TilePosition& position) const
	{
		static const std::vector<ChessPieceId> EMPTY_POSITION_HITTERS;

		auto it = m_positionHitters.find(position);
		if (it != m_positionHitters.end())
		{
			return it->second;
		}

		return EMPTY_POSITION_HITTERS;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const TilePositionSet& PlayerPossibleMoves::GetPlayerPossibleMoves() const
	{
		if (m_Cache.m_isDirty)
		{
			m_Cache.m_playerMoves = CollectPlayerPossibleMoves();
			m_Cache.m_isDirty = false;
		}

		return m_Cache.m_playerMoves;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerPossibleMoves::InvalidatePlayerPossibleMovesCache()
	{
		m_Cache.m_isDirty = true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerPossibleMoves::RemoveHitterFromPosition(ChessPieceId hitter, const TilePosition& position)
	{
		auto it = m_positionHitters.find(position);
		if (it == m_positionHitters.end())
		{
			return;
		}

		auto& positionHitters = it->second;
		auto newEnd = std::remove(positionHitters.begin(), positionHitters.end(), hitter);
		positionHitters.erase(newEnd, positionHitters.end());
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	TilePositionSet PlayerPossibleMoves::CollectPlayerPossibleMoves() const
	{
		TilePositionSet result;
		for (const auto& [chessPieceId, possibleMoves] : m_possibleMoves)
		{
			for (const auto& position : possibleMoves)
			{
				result.insert(position);
			}
		}

		return result;
	}
}