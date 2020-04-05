#pragma once
#include "io/BaseOutputDevice.h"

namespace chess
{
	enum class ETurnState;
	class ChessBoard;

	class ConsoleChessOutputDevice : public mvc::BaseOutputDevice
	{
	public:
		void ClearConsole();
		void RenderChessboard(ChessBoard& chessboard);
		void RenderTurnState(ETurnState turnState);
	};
}