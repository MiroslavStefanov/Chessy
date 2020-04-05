#include "stdafx.h"
#include "ConsoleChessOutputDevice.h"
#include "ChessBoard.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::ClearConsole()
	{
		system("CLS");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderChessboard(ChessBoard& chessboard)
	{
		chessboard.Draw();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderTurnState(ETurnState turnState)
	{
		switch (turnState)
		{
		case chess::ETurnState::EndTurn:
			std::cout<<"EndTurn";
			break;
		case chess::ETurnState::Select:
			std::cout << "Select";
			break;
		case chess::ETurnState::Unselect:
			std::cout << "Unselect";
			break;
		case chess::ETurnState::ErrorState:
			std::cout << "ErrorState";
			break;
		}

		std::cout << std::endl;
	}
}