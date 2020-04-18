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
		TilePosition ParseTilePosition(const std::string& input) const;
		bool IsValidRow(char row) const;
		bool IsValidColumn(char column) const;

	private:
		TilePosition m_inputPosition;
	};
}