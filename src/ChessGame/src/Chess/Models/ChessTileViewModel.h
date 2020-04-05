#pragma once
#include "Utils/Utils.h"

namespace chess
{
	struct ChessTileViewModel
	{
		ChessTileViewModel();
		ChessTileViewModel(ChessTileViewModel&& other) noexcept;
		~ChessTileViewModel();
		EColor Color;
		ChessPieceId Piece;
		bool IsPicked;
		std::vector<std::unique_ptr<mvc::Event>> OnClickEvents;
	};
}