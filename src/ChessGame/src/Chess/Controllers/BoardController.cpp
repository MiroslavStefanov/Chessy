#include "stdafx.h"
#include "BoardController.h"

#include "mvc/ModelAndView.h"
#include "StringHash.h"

#include "Events/EventTypes.h"
#include "Events/CellClickedEvent.h"

#include "Views/ChessViews.h"
#include "Models/ChessboardViewModel.h"
#include "Services/BoardService.h"
#include "Services/PlayerService.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	BoardController::BoardController(BoardService& boardService, PlayerService& playerService)
		: m_boardService(boardService)
		, m_playerService(playerService)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BoardController::RegisterConsumers()
	{
		BaseController::RegisterConsumers();
		//RegisterConsumer<CellClickedEvent>(std::bind(&BoardController::OnCellClickedEvent, this, std::placeholders::_1));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView BoardController::OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event)
	{
		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(mvc::ViewId((int)ViewType::Chessboard));
		modelAndView.SetModel(STRING_ID("chessboard"), CreateChessboardViewModel());
		return modelAndView;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::Model> BoardController::CreateChessboardViewModel() const
	{
		int tileIndex = 0;
		EColor tileColor = EColor::Black;
		auto tileViewModelTransform = [&tileColor, &tileIndex, this](ChessPieceId pieceId) -> ChessTileViewModel
		{
			ChessTileViewModel viewModel;
			viewModel.Color = tileColor;
			viewModel.Piece = pieceId;
			viewModel.IsPicked = m_playerService.IsPicked(pieceId);
			viewModel.OnClickEvents = m_playerService.GenerateEventsForTile(TilePosition(Position(tileIndex)), pieceId);

			tileColor = GetAlternateColor(tileColor);
			++tileIndex;
			return viewModel;
		};

		auto model = std::make_unique<ChessboardViewModel>();
		auto boardState = m_boardService.GetBoardState();
		std::transform(boardState.begin(), boardState.end(), std::back_inserter(model->Tiles), tileViewModelTransform);
		return model;
	}
}