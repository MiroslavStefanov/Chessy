#include "stdafx.h"
#include "ChessboardView.h"
#include "App/ConsoleChess/ConsoleChessInputDevice.h"
#include "App/ConsoleChess/ConsoleChessOutputDevice.h"
#include "Events/CellClickedEvent.h"
#include "mvc/ModelAndView.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::Update(mvc::ModelAndView&& modelAndView)
	{
		const StringId modelId = modelAndView.GetModelId();
		if (modelId == STRING_ID("chessboard"))
		{
			UpdateBoardModel(std::move(modelAndView));
		}
		else if (modelId == STRING_ID("playerTurn"))
		{
			UpdateTurnModel(std::move(modelAndView));
		}
		else
		{
			assert(false);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessInput(mvc::BaseInputDevice* inputDevice)
	{
		auto device = dynamic_cast<ConsoleChessInputDevice*>(inputDevice);
		if (!device)
			return;

		TilePosition inputPosition = device->PollTilePosition();
		if (inputPosition.IsValid())
		{
			auto& events = m_boardModel.Tiles[inputPosition.AsIndex()].OnClickEvents;
			for (auto& event : events)
			{
				device->AddEvent(std::move(event));
			}
			events.clear();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessOutput(mvc::BaseOutputDevice* outputDevice)
	{
		auto device = dynamic_cast<ConsoleChessOutputDevice*>(outputDevice);
		if (!device)
			return;

		device->ClearConsole();
		device->RenderChessboard(m_boardModel);
		device->RenderPlayerTurn(m_turnModel);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::UpdateBoardModel(mvc::ModelAndView&& modelAndView)
	{
		auto boardModel = modelAndView.GetModel<ChessboardViewModel>();
		if (boardModel)
		{
			m_boardModel = std::move(*boardModel);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::UpdateTurnModel(mvc::ModelAndView&& modelAndView)
	{
		auto turnModel = modelAndView.GetModel<PlayerTurnViewModel>();
		if (turnModel)
		{
			m_turnModel = std::move(*turnModel);
		}
	}
}