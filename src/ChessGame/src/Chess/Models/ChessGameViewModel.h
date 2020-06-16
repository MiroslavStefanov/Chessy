#pragma once
#include "mvc/Model.h"
#include "Models/TilePositionViewModel.h"
#include "Models/ChessTileViewModel.h"

namespace chess
{
	struct ChessGameViewModel : public mvc::Model
	{
		ETurnState TurnState = ETurnState::ErrorState;
		EColor ActivePlayerColor = EColor::Colorless;
		ChessPieceId PickedPieceId = ChessPieceId::Invalid();
		std::vector<TilePositionViewModel> PossibleMoves;
		std::vector<ChessTileViewModel> ChessBoard;
		bool ActivePlayerInCheck = false;
	};
}