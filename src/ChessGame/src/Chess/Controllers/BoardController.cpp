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
	BoardController::BoardController()
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BoardController::RegisterConsumers()
	{
		BaseController::RegisterConsumers();
		RegisterConsumer<CellClickedEvent>(std::bind(&BoardController::OnCellClickedEvent, this, std::placeholders::_1));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView BoardController::OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event)
	{
		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(mvc::ViewId((int)ViewType::Chessboard));
		modelAndView.SetModel(STRING_ID("chessboard"), CreateChessboardViewModel());
		return modelAndView;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView BoardController::OnCellClickedEvent(CellClickedEvent const& event)
	{
		//TODO: remove this event and add concrete events (pick piece up, drop piece, move piece) and update board sevice accordingly
		auto playerService = GetDependency<PlayerService>();
		auto pickedPieceId = playerService->GetPickedPiece();
		if (!pickedPieceId.IsValid())
		{
			return mvc::ModelAndView::Invalid();
		}

		auto boardService = GetDependency<BoardService>();
		boardService->MoveChessPiece(pickedPieceId, event.Position);
		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(mvc::ViewId((int)ViewType::Chessboard));
		modelAndView.SetModel(STRING_ID("chessboard"), CreateChessboardViewModel());
		return modelAndView;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::Model> BoardController::CreateChessboardViewModel() const
	{
		int tileIndex = 0;
		EColor tileColor = EColor::White;
		const ChessPieceId pickedPieceId = GetDependency<PlayerService>()->GetPickedPiece();
		auto tileViewModelTransform = [&tileColor, &pickedPieceId, &tileIndex](ChessPieceId pieceId) -> ChessTileViewModel
		{
			if (tileIndex++ % 8 != 0)
			{
				tileColor = GetAlternateColor(tileColor);
			}

			ChessTileViewModel viewModel;
			viewModel.Color = tileColor;
			viewModel.Piece = pieceId;
			viewModel.IsPicked = pieceId.IsValid() && pieceId == pickedPieceId;

			return viewModel;
		};

		auto model = std::make_unique<ChessboardViewModel>();
		auto boardState = GetDependency<BoardService>()->GetBoardState();
		std::transform(boardState.begin(), boardState.end(), std::back_inserter(model->Tiles), tileViewModelTransform);
		return model;
	}
}