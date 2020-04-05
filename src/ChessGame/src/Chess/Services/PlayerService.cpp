#include "stdafx.h"
#include "PlayerService.h"
#include "event/Event.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerService::PlayerService(ChessPieceRegistry const& pieceRepository) : m_pieceRepository(pieceRepository)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::vector<TilePosition> PlayerService::GetPossibleMoves() const
	{
		return std::vector<TilePosition>();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool PlayerService::IsPicked(ChessPieceId pieceId) const
	{
		auto it = m_pickedPieces.find(m_activePlayerColor);
		if (it == m_pickedPieces.end())
			return false;

		return pieceId == it->second;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::vector<std::unique_ptr<mvc::Event>> PlayerService::GenerateEventsForTile(const TilePosition& tilePosition, const ChessPieceId& tilePiece) const
	{
		return std::vector<std::unique_ptr<mvc::Event>>();
	}

}