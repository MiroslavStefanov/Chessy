#pragma once
#include "io/InputDevice.h"
#include "Utils/Utils.h"

namespace chess
{
	class TilePosition;

	class ConsoleChessInputDevice : public mvc::InputDevice
	{
	public:
		ConsoleChessInputDevice();

		void Update() override;

		const TilePosition& GetInputTilePosition();

	private:
		TilePosition m_inputPosition;
	};
}