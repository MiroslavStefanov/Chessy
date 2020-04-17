#include "stdafx.h"
#include "PlayerService.h"
#include "event/Event.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerService::PlayerService()
		: m_pickedPiece(ChessPieceId::Invalid())
		, m_activePlayerColor(EColor::Colorless)
		, m_turnState(ETurnState::ErrorState)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::vector<TilePosition> PlayerService::GetPossibleMoves() const
	{
		return std::vector<TilePosition>(); //TODO: implement
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceId PlayerService::GetPickedPiece() const
	{
		return m_pickedPiece;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ETurnState PlayerService::GetTurnState() const
	{
		return m_turnState;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::StartGame()
	{
		m_activePlayerColor = EColor::White;
		m_turnState = ETurnState::StartGame;
	}
}