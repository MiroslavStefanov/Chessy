#include "stdafx.h"
#include "PlayerService.h"
#include "ChessPieces/ChessPieceTypes.h"
#include "Utils/Utils.h"

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
		assert(!m_pickedPiece.IsValid() && pieceId.IsValid());

		m_pickedPiece = pieceId;
		m_turnState = ETurnState::Select;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerService::DropChessPiece()
	{
		assert(m_pickedPiece.IsValid());

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