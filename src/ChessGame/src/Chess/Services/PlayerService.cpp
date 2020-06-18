#include "stdafx.h"
#include "PlayerService.h"
#include "ChessPieces/ChessPieceTypes.h"
#include "Utils/Utils.h"

#include "ErrorHandling/Exceptions/InvalidChessPieceException.h"
#include "ErrorHandling/Exceptions/InvalidPlayerColorException.h"
#include "ErrorHandling/Exceptions/ChessPieceAlreadyPickedException.h"
#include "ErrorHandling/Exceptions/ChessPieceNotPickedException.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerService::PlayerService()
		: m_pickedPiece(ChessPieceId::Invalid())
		, m_activePlayerColor(EColor::Colorless)
		, m_turnState(ETurnState::ErrorState)
		, m_playerCheckStates{
			{EColor::White, EPlayerCheckState::Clear},
			{EColor::Black, EPlayerCheckState::Clear}
		}
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
	bool PlayerService::IsActivePlayerInCheck() const
	{
		try
		{
			return m_playerCheckStates[m_activePlayerColor] == EPlayerCheckState::Check;
		}
		catch (std::out_of_range & e)
		{
			throw InvalidPlayerColorException(e);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::SetActivePlayerCheckState(EPlayerCheckState checkState)
	{
		assert(m_playerCheckStates.HasKey(m_activePlayerColor));
		m_playerCheckStates[m_activePlayerColor] = checkState;
		
		if (checkState == EPlayerCheckState::Checkmate)
		{
			m_turnState = ETurnState::GameOver;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool PlayerService::CanPickChessPiece(ChessPieceId pieceId) const
	{
		return !m_pickedPiece.IsValid() && pieceId.IsValid() && pieceId.GetColor() == m_activePlayerColor;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool PlayerService::CanDropChessPiece() const
	{
		return m_pickedPiece.IsValid() && m_pickedPiece.GetColor() == m_activePlayerColor;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::StartGame()
	{
		m_activePlayerColor = EColor::White;
		m_turnState = ETurnState::StartGame;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::PickChessPiece(ChessPieceId pieceId)
	{
		if (!pieceId.IsValid())
		{
			throw InvalidChessPieceException(pieceId);
		}

		if (m_pickedPiece.IsValid())
		{
			throw ChessPieceAlreadyPickedException();
		}

		m_pickedPiece = pieceId;
		m_turnState = ETurnState::Select;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::DropChessPiece()
	{
		if (!m_pickedPiece.IsValid())
		{
			throw ChessPieceNotPickedException();
		}

		m_pickedPiece = ChessPieceId::Invalid();
		m_turnState = ETurnState::Unselect;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::OnChessPieceMovedToPosition(ChessPieceId pieceId, const TilePosition& position)
	{
		if (pieceId.GetType() == EChessPieceType::Pawn && IsPawnPromotionPosition(pieceId.GetColor(), position))
		{
			m_turnState = ETurnState::PawnPromotion;
		}
		else
		{
			EndTurn();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::OnPawnPromoted()
	{
		EndTurn();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::EndTurn()
	{
		m_pickedPiece = ChessPieceId::Invalid();
		m_activePlayerColor = GetAlternateColor(m_activePlayerColor);
		m_turnState = ETurnState::EndTurn;
	}
}