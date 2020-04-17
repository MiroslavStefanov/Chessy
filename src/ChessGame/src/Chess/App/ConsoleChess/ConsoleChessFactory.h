#pragma once
#include "App/ChessFactory.h"

namespace chess
{
	class ConsoleChessFactory : public ChessFactory
	{
	public:
		std::unique_ptr<mvc::InputDevice> CreateInputDevice() const override;
		std::unique_ptr<mvc::OutputDevice> CreateOutputDevice() const override;
		std::unique_ptr<mvc::View> CreateView(ViewType viewType) const override;
	};
}