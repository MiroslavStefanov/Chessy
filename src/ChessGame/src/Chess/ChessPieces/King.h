#pragma once
#include "ChessPiece.h"

namespace chess
{
	class King :
		public ChessPiece
	{
	public:
		King(EColor color);
	};
}
