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
			possibleMoves.push_back(pieceMovementIterator->GetTilePosition());
			pieceMovementIterator->Advance();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerPossibleMoves::RemoveChessPiecePossibleMoves(ChessPieceId pieceId)
	{
		m_possibleMoves.erase(pieceId);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	const std::vector<TilePosition>& PlayerPossibleMoves::GetChessPiecePossibleMoves(ChessPieceId pieceId) const
	{
		return m_possibleMoves.at(pieceId);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	TilePositionSet PlayerPossibleMoves::GetPlayerPossibleMoves() const
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