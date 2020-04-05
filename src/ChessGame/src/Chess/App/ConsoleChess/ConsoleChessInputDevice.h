#pragma once
#include "io/BaseInputDevice.h"

namespace chess
{
	class TilePosition;

	class ConsoleChessInputDevice : public mvc::BaseInputDevice
	{
	public:
		TilePosition PollTilePosition();
	};
}