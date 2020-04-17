#include "stdafx.h"
#include "ChessboardView.h"
#include "App/ConsoleChess/ConsoleChessInputDevice.h"
#include "App/ConsoleChess/ConsoleChessOutputDevice.h"
#include "Events/CellClickedEvent.h"
#include "mvc/ModelAndView.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::SetModel(StringId modelId, std::unique_ptr<mvc::Model>&& model)
	{
		std::unique_ptr<mvc::Model> localModel = std::move(model);
		if (modelId == STRING_ID("chessboard"))
		{
			UpdateBoardModel(dynamic_cast<ChessboardViewModel*>(localModel.get()));
		}
		else if (modelId == STRING_ID("playerTurn"))
		{
			UpdateTurnModel(dynamic_cast<PlayerTurnViewModel*>(localModel.get()));
		}
		else
		{
			assert(false);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessInput(mvc::InputDevice* inputDevice)
	{
		auto device = dynamic_cast<ConsoleChessInputDevice*>(inputDevice);
		if (!device)
			return;

		auto& inputPosition = device->GetInputTilePosition();
		if (!inputPosition.IsValid())
		{
			return;
		}

		RaiseEvent(CellClickedEvent(inputPosition));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessOutput(mvc::OutputDevice* outputDevice)
	{
		auto device = dynamic_cast<ConsoleChessOutputDevice*>(outputDevice);
		if (!device)
			return;

		device->RenderChessboard(m_boardModel);
		device->RenderPlayerTurn(m_turnModel);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::UpdateBoardModel(ChessboardViewModel* viewModel)
	{
		if (!viewModel)
		{
			assert(false);
			return;
		}
		m_boardModel = std::move(*viewModel);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::UpdateTurnModel(PlayerTurnViewModel* viewModel)
	{
		if (!viewModel)
		{
			assert(false);
			return;
		}
		m_turnModel = std::move(*viewModel);
	}
}