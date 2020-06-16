#include "stdafx.h"
#include "ConsoleChessFactory.h"
#include "ConsoleChessInputDevice.h"
#include "ConsoleChessOutputDevice.h"
#include "Views/ChessViews.h"
#include "Views/ChessboardView.h"
#include "Views/GameOverView.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::InputDevice> ConsoleChessFactory::CreateInputDevice() const
	{
		return std::make_unique<ConsoleChessInputDevice>();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::OutputDevice> ConsoleChessFactory::CreateOutputDevice() const
	{
		return std::make_unique<ConsoleChessOutputDevice>();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::View> ConsoleChessFactory::CreateView(ViewType viewType) const
	{
		switch (viewType)
		{
		case ViewType::Chessboard:
			return std::make_unique<ChessboardView>();
		case ViewType::GameOver:
			return std::make_unique<GameOverView>();
		default:
			assert(!"Unknown view type in ConsoleChessFactory::CreateView");
		}

		return nullptr;
	}

}