#include "stdafx.h"
#include "GameOverView.h"
#include "App/ConsoleChess/ConsoleChessInputDevice.h"
#include "App/ConsoleChess/ConsoleChessOutputDevice.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void GameOverView::SetModel(std::unique_ptr<mvc::Model>&& model)
	{
		std::unique_ptr<mvc::Model> localModel = std::move(model);
		auto gameOverViewModel = dynamic_cast<GameOverViewModel*>(localModel.get());
		LogReturnIf(!gameOverViewModel, VOID_RETURN);


		m_model = std::move(*gameOverViewModel);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void GameOverView::ProcessInput(mvc::InputDevice* inputDevice)
	{
		StopApplication();
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