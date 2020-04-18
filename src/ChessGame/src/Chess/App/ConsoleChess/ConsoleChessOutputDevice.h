#pragma once
#include "io/OutputDevice.h"
#include "dependency/Depender.h"
#include "ChessPieces/ChessPieceRegistry.h"

#include <sstream>

namespace chess
{
	enum class ETurnState;
	struct ChessboardViewModel;
	struct ChessTileViewModel;
	struct PlayerTurnViewModel;
	class ChessPieceRegistry;

	class ConsoleChessOutputDevice : public mvc::OutputDevice, public mvc::Depender<ChessPieceRegistry>
	{
	public:
		void Update() override;

		void RenderChessboard(const ChessboardViewModel& chessboard);
		void RenderPlayerTurn(const PlayerTurnViewModel& playerTurn);

	private:
		void ClearConsole();

		void RenderChessboardRow(std::vector<ChessTileViewModel>::const_iterator rowBegin, int rowNumber);
		void RenderBorderRow();
		void RenderLettersRow();

		char GetTileCenterVisual(const ChessTileViewModel& tile) const;

	private:
		std::stringstream m_frameBuffer;
	};
}