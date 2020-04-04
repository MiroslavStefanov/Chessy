#include "stdafx.h"
#include "ChessboardView.h"
#include "ChessBoard.h"
#include "mvc/ModelAndView.h"
#include "io/BaseOutputDevice.h"

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
	void ChessboardView::Render(mvc::BaseOutputDevice& outputDevice)
	{
		if (m_dataModel.Chessboard)
		{
			m_dataModel.Chessboard->Draw();
		}
		outputDevice.RenderText(TurnStateToString(m_dataModel.TurnState) + "\n");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string ChessboardView::TurnStateToString(ETurnState turnState) const
	{
		switch (turnState)
		{
		case chess::ETurnState::EndTurn:
			return "EndTurn";
		case chess::ETurnState::Select:
			return "Select";
		case chess::ETurnState::Unselect:
			return "Unselect";
		case chess::ETurnState::ErrorState:
			return "ErrorState";
		default:
			return "";
		}
	}

}