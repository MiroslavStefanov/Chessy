#include "stdafx.h"
#include "ModelMapper.h"

#include "Models/TilePositionViewModel.h"
#include "Models/ChessTileViewModel.h"
#include "Utils/Utils.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	TilePositionViewModel ModelMapper::MapTilePositionView(const TilePosition& tilePostion)
	{
		TilePositionViewModel model;
		auto& position = tilePostion.AsPosition();
		model.Row = position.row;
		model.Column = position.col;
		return model;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::vector<ChessTileViewModel> ModelMapper::ProduceChessBoardView(const std::vector<ChessPieceId>& chessBoardState, ChessPieceId pickedPieceId)
	{
		const auto boardTilesSize = chessBoardState.size();
		std::vector<ChessTileViewModel> result(boardTilesSize);
		
		EColor chessTileColor = EColor::White;
		for (std::size_t i = 0; i < boardTilesSize; ++i)
		{
			if (i % CHESS_BOARD_SIDE != 0)
			{
				chessTileColor = GetAlternateColor(chessTileColor);
			}

			auto& viewModel = result[i];
			viewModel.Color = chessTileColor;
			viewModel.Piece = chessBoardState[i];
			viewModel.IsPicked = pickedPieceId.IsValid() && chessBoardState[i] == pickedPieceId;
		}

		return result;
	}
}