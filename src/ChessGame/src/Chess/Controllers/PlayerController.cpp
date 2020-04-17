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
	PlayerController::PlayerController()
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void PlayerController::RegisterConsumers()
	{
		BaseController::RegisterConsumers();
		RegisterConsumer<CellClickedEvent>(std::bind(&PlayerController::OnCellClickedEvent, this, std::placeholders::_1));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView PlayerController::OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event)
	{
		auto playerService = GetDependency<PlayerService>();
		playerService->StartGame();
		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(ViewTypeToId(ViewType::Chessboard));
		modelAndView.SetModel(STRING_ID("playerTurn"), CreatePlayerTurnViewModel(ETurnState::StartGame));
		return modelAndView;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView PlayerController::OnCellClickedEvent(const CellClickedEvent& event)
	{
		auto playerService = GetDependency<PlayerService>();
		//TODO: remove this event and add concrete events (pick piece up, drop piece, move piece) and update player sevice accordingly
		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(ViewTypeToId(ViewType::Chessboard));
		modelAndView.SetModel(STRING_ID("playerTurn"), CreatePlayerTurnViewModel(playerService->GetTurnState()));
		return modelAndView;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::Model> PlayerController::CreatePlayerTurnViewModel(ETurnState turnState) const
	{
		auto model = std::make_unique<PlayerTurnViewModel>();
		model->TurnState = turnState;
		auto possibleMoves = GetDependency<PlayerService>()->GetPossibleMoves();
		std::transform(possibleMoves.begin(), possibleMoves.end(), std::back_inserter(model->PossibleMoves), &ModelMapper::MapTilePositionView);
		return model;
	}
}