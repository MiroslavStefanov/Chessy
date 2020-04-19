#pragma once
#include "mvc/Model.h"
#include "Models/TilePositionViewModel.h"
#include "Models/ChessTileViewModel.h"

namespace chess
{
	enum class ETurnState;
	enum class EColor;

	struct ChessGameViewModel : public mvc::Model
	{
		ETurnState TurnState;
		EColor ActivePlayerColor;
		ChessPieceId PickedPieceId;
		std::vector<TilePositionViewModel> PossibleMoves;
		std::vector<ChessTileViewModel> ChessBoard;
	};
}