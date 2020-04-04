#pragma once
#include "App/ChessFactory.h"

namespace chess
{
	class ConsoleChessFactory : public ChessFactory
	{
	public:
		std::unique_ptr<mvc::BaseInputDevice> CreateInputDevice() const override;
		std::unique_ptr<mvc::BaseOutputDevice> CreateOutputDevice() const override;
		std::unique_ptr<mvc::View> CreateView(ViewType viewType) const override;
	};
}