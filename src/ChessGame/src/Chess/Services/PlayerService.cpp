#include "stdafx.h"
#include "PlayerService.h"
#include "event/Event.h"
#include "ChessPieces/ChessPieceRegistry.h"

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
	EColor PlayerService::GetActivePlayerColor() const
	{
		return m_activePlayerColor;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::StartGame()
	{
		m_activePlayerColor = EColor::White;
		m_turnState = ETurnState::StartGame;
	}
}