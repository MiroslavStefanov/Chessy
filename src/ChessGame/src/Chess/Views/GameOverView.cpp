#include "stdafx.h"
#include "GameOverView.h"
#include "App/ConsoleChess/ConsoleChessInputDevice.h"
#include "App/ConsoleChess/ConsoleChessOutputDevice.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void GameOverView::SetModel(StringId modelId, std::unique_ptr<mvc::Model>&& model)
	{
		std::unique_ptr<mvc::Model> localModel = std::move(model);
		auto gameOverViewModel = dynamic_cast<GameOverViewModel*>(localModel.get());
		if (!gameOverViewModel)
		{
			assert(false);
			return;
		}

		m_model = std::move(*gameOverViewModel);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void GameOverView::ProcessInput(mvc::InputDevice* inputDevice)
	{
		//do nothing
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void GameOverView::ProcessOutput(mvc::OutputDevice* outputDevice)
	{
		auto device = dynamic_cast<ConsoleChessOutputDevice*>(outputDevice);
		if (!device)
			return;

		device->RenderChessboard(m_model.ChessBoard);
		device->RenderGameOverMessage(m_model.WinnerColor);
	}
}