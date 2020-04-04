#pragma once
#include "Rook.h"
#include "Bishop.h"

namespace chess
{
	class Queen :
		public Rook,
		public Bishop
	{
	public:
		Queen(EColor color);
	};
}
