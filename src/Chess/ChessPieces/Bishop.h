#pragma once
#include "ChessPiece.h"

namespace chess
{
	class Bishop :
		virtual public ChessPiece
	{
	public:
		Bishop(EColor color);
	};
}

