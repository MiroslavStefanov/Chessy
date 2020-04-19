#pragma once
#include "Utils/Utils.h"
#include "dependency/Depender.h"

namespace chess
{
	class ChessPieceRegistry;

	class PlayerService : public mvc::Depender<ChessPieceRegistry>
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

	private:
		void EndTurn();

	private:
		ChessPieceId m_pickedPiece;
		EColor m_activePlayerColor;
		ETurnState m_turnState;
	};
}

