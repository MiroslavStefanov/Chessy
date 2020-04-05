#include "stdafx.h"
#include "ChessboardView.h"
#include "ChessBoard.h"
#include "App/ConsoleChess/ConsoleChessInputDevice.h"
#include "App/ConsoleChess/ConsoleChessOutputDevice.h"
#include "Events/CellClickedEvent.h"
#include "mvc/ModelAndView.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::Update(const mvc::ModelAndView& modelAndView)
	{
		auto chessboardViewModel = modelAndView.GetModel<ChessboardViewModel>();
		if (chessboardViewModel)
		{
			m_dataModel = *chessboardViewModel;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessInput(mvc::BaseInputDevice* inputDevice)
	{
		auto device = dynamic_cast<ConsoleChessInputDevice*>(inputDevice);
		if (!device)
			return;

		TilePosition inputPosition = device->PollTilePosition();
		device->AddEvent(std::make_unique<CellClickedEvent>(inputPosition));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessOutput(mvc::BaseOutputDevice* outputDevice)
	{
		auto device = dynamic_cast<ConsoleChessOutputDevice*>(outputDevice);
		if (!device)
			return;

		device->ClearConsole();

		if (m_dataModel.Chessboard)
		{
			device->RenderChessboard(*m_dataModel.Chessboard);
		}

		device->RenderTurnState(m_dataModel.TurnState);
	}
}