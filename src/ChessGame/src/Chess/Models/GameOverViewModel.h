#pragma once
#include "mvc/Model.h"
#include "Models/ChessTileViewModel.h"

namespace chess
{
	struct GameOverViewModel : public mvc::Model
	{
		EColor WinnerColor = EColor::Colorless;
		std::vector<ChessTileViewModel> ChessBoard;
	};
}