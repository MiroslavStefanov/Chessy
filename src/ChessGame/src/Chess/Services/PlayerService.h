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

		void StartGame();

	private:
		ChessPieceId m_pickedPiece;
		EColor m_activePlayerColor;
		ETurnState m_turnState;
	};
}

