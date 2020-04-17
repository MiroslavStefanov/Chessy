#pragma once
#include "Utils/Utils.h"
#include "dependency/Depender.h"
#include "ChessPieces/ChessPieceRegistry.h"

namespace chess
{
	class ChessPieceRegistry;

	class PlayerService : public mvc::Depender<ChessPieceRegistry>
	{
	public:
		PlayerService();

		std::vector<TilePosition> GetPossibleMoves() const;
		ChessPieceId GetPickedPiece() const;
		ETurnState GetTurnState() const;

		void StartGame();

	private:
		ChessPieceId m_pickedPiece;
		EColor m_activePlayerColor;
		ETurnState m_turnState;
	};
}

