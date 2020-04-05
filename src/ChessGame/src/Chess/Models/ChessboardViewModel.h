#pragma once
#include "mvc/Model.h"
#include "Models/ChessTileViewModel.h"

namespace chess
{
	struct ChessboardViewModel : public mvc::Model
	{
		std::vector<ChessTileViewModel> Tiles;
	};
}