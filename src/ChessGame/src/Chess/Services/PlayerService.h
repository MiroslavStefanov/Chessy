#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class PlayerService
	{
	public:
		PlayerService();

		ChessPieceId GetPickedPiece() const;
		ETurnState GetTurnState() const;
		EColor GetActivePlayerColor() const;

		bool CanPickChessPiece(ChessPieceId pieceId) const;
		bool CanDropChessPiece() const;

		void StartGame();
		void PickChessPiece(ChessPieceId pieceId);
		void DropChessPiece();
		void OnChessPieceMovedToPosition(ChessPieceId pieceId, const TilePosition& position);
		void OnPawnPromoted();

	private:
		void EndTurn();

	private:
		ChessPieceId m_pickedPiece;
		EColor m_activePlayerColor;
		ETurnState m_turnState;
	};
}

