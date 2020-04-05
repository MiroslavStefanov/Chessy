#pragma once
#include "io/BaseOutputDevice.h"

namespace chess
{
	enum class ETurnState;
	struct ChessboardViewModel;
	struct ChessTileViewModel;
	struct PlayerTurnViewModel;
	class ChessPieceRegistry;

	class ConsoleChessOutputDevice : public mvc::BaseOutputDevice
	{
	public:
		void ClearConsole();
		void RenderChessboard(const ChessboardViewModel& chessboard);
		void RenderPlayerTurn(const PlayerTurnViewModel& playerTurn);

		void SetChessPieceRegistry(const ChessPieceRegistry* registry);

	private:
		void RenderChessTile(const ChessTileViewModel& tile);
		void RenderTurnState(ETurnState state);

	private:
		const ChessPieceRegistry* m_pieceRegistry = nullptr;
	};
}