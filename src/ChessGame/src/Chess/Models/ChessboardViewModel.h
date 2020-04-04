#pragma once
#include "mvc/Model.h"
#include "Utils/Utils.h"

namespace chess
{
	class ChessBoard;

	struct ChessboardViewModel : public mvc::Model
	{
		ChessBoard* Chessboard;
		ETurnState TurnState;
	};
}