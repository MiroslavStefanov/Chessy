#include "stdafx.h"
#include "PlayerController.h"

#include "mvc/ModelAndView.h"
#include "StringHash.h"

#include "ModelMapper.h"

#include "Events/EventTypes.h"
#include "Events/CellClickedEvent.h"

#include "Views/ChessViews.h"
#include "Models/PlayerTurnViewModel.h"
#include "Services/PlayerService.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	PlayerController::PlayerController(PlayerService& playerService) : m_playerService(playerService)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void PlayerController::RegisterConsumers()
	{
		BaseController::RegisterConsumers();
		//RegisterConsumer<CellClickedEvent>(std::bind(&PlayerController::OnCellClickedEvent, this, std::placeholders::_1));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView PlayerController::OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event)
	{
		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(ViewTypeToId(ViewType::Chessboard));
		modelAndView.SetModel(STRING_ID("playerTurn"), CreatePlayerTurnViewModel(ETurnState::StartGame));
		return modelAndView;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::Model> PlayerController::CreatePlayerTurnViewModel(ETurnState turnState) const
	{
		auto model = std::make_unique<PlayerTurnViewModel>();
		model->TurnState = turnState;
		auto possibleMoves = m_playerService.GetPossibleMoves();
		std::transform(possibleMoves.begin(), possibleMoves.end(), std::back_inserter(model->PossibleMoves), &ModelMapper::MapTilePositionView);
		return model;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	/*mvc::ModelAndView PlayerController::OnCellClickedEvent(const CellClickedEvent& event)
	{
		auto state = ChessyTurn(event.Position);
		if (state == chess::ETurnState::EndTurn)
		{
			OnEndTurn();
		}

		return MakeChessboardView(state);
	}*/
}