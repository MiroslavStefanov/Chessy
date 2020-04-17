#pragma once
#include "Utils/Utils.h"

namespace chess
{
	struct ChessTileViewModel
	{
		EColor Color;
		ChessPieceId Piece;
		bool IsPicked;
	};
}