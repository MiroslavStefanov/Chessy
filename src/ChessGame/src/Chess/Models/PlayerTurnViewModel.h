#pragma once
#include "mvc/Model.h"
#include "TilePositionViewModel.h"

namespace chess
{
	enum class ETurnState;
	enum class EColor;

	struct PlayerTurnViewModel : public mvc::Model
	{
		ETurnState TurnState;
		std::vector<TilePositionViewModel> PossibleMoves;
		EColor ActivePlayerColor;
		ChessPieceId PickedPieceId;
	};
}