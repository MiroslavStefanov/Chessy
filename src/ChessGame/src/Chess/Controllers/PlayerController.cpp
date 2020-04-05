#include "stdafx.h"
#include "PlayerController.h"

#include "mvc/ModelAndView.h"
#include "StringHash.h"

#include "ChessBoard.h"
#include "ChessboardTile.h"
#include "ChessPieces/ChessPiece.h"

#include "Events/EventTypes.h"
#include "Events/CellClickedEvent.h"

#include "Views/ChessViews.h"
#include "Models/ChessboardViewModel.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void PlayerController::RegisterConsumers()
	{
		BaseController::RegisterConsumers();
		RegisterConsumer<CellClickedEvent>(std::bind(&PlayerController::OnCellClickedEvent, this, std::placeholders::_1));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView PlayerController::OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event)
	{
		return MakeChessboardView(ETurnState::StartGame);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView PlayerController::OnCellClickedEvent(const CellClickedEvent& event)
	{
		auto state = ChessyTurn(event.Position);
		if (state == chess::ETurnState::EndTurn)
		{
			OnEndTurn();
		}

		return MakeChessboardView(state);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView PlayerController::MakeChessboardView(ETurnState turnState) const
	{
		auto model = std::make_unique<ChessboardViewModel>();
		model->Chessboard = m_chessBoard;
		model->TurnState = turnState;

		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(mvc::ViewId((int)ViewType::Chessboard));
		modelAndView.SetModel(STRING_ID("chessboard"), std::move(model));
		return modelAndView;
	}
}