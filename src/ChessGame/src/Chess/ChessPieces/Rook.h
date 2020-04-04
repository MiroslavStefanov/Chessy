#pragma once
#include "ChessPiece.h"

namespace chess
{
	class Rook :
		virtual public ChessPiece
	{
	public:
		Rook(EColor color);
	};
}
