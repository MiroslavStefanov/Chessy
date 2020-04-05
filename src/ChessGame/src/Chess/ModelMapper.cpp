#include "stdafx.h"
#include "ModelMapper.h"

#include "Models/TilePositionViewModel.h"
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
}