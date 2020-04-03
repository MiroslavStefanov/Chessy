#pragma once
#include "ChessPiece.h"

namespace chess
{
	class Pawn :
		public ChessPiece
	{
	public:
		Pawn(EColor color);
	};
}
