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

		const TilePosition& GetTilePosition() const;
		EChessPieceType GetChessPieceType() const;

	private:
		TilePosition ParseTilePosition(const std::string& input) const;
		bool IsValidRow(char row) const;
		bool IsValidColumn(char column) const;

		EChessPieceType ParseChessPieceType(const std::string& input) const;
		bool CaseInsensitiveEquals(const std::string& lhs, const std::string& rhs) const;

	private:
		TilePosition m_inputPosition;
		EChessPieceType m_inputPieceType;
	};
}