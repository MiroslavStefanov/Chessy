#include "stdafx.h"
#include "App/ChessApplication.h"
#include "App/ConsoleChess/ConsoleChessFactory.h"

using namespace chess;

int main()
{
	ConsoleChessFactory chessFactory;
	ChessApplication application(chessFactory);

	application.Initialize();
	application.Start();
}