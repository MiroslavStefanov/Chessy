#include "stdafx.h"
#include "ChessGameController.h"

#include "mvc/ModelAndView.h"
#include "StringHash.h"

#include "ModelMapper.h"

#include "Events/EventTypes.h"
#include "Events/ChessPieceDroppedEvent.h"
#include "Events/ChessPiecePickedEvent.h"
#include "Events/ChessPieceMovedEvent.h"
#include "Events/PawnPromotedEvent.h"

#include "Views/ChessViews.h"
#include "Models/ChessGameViewModel.h"
#include "Services/PlayerService.h"
#include "Services/BoardService.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessGameController::RegisterConsumers()
	{
		BaseController::RegisterConsumers();
		RegisterConsumer<ChessPiecePickedEvent>(std::bind(&ChessGameController::OnChessPiecePickedEvent, this, std::placeholders::_1));
		RegisterConsumer<ChessPieceDroppedEvent>(std::bind(&ChessGameController::OnChessPieceDroppedEvent, this, std::placeholders::_1));
		RegisterConsumer<ChessPieceMovedEvent>(std::bind(&ChessGameController::OnChessPieceMovedEvent, this, std::placeholders::_1));
		RegisterConsumer<PawnPromotedEvent>(std::bind(&ChessGameController::OnPawnPromotedEvent, this, std::placeholders::_1));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event)
	{
		GetDependency<PlayerService>().StartGame();
		return CreateChessboardModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::OnChessPiecePickedEvent(ChessPiecePickedEvent const& event)
	{
		if (!CanPickChessPiece(event.PieceId))
		{
			//TODO: error handling
			return mvc::ModelAndView::Invalid();
		}

		GetDependency<PlayerService>().PickChessPiece(event.PieceId);
		return CreateChessboardModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::OnChessPieceDroppedEvent(ChessPieceDroppedEvent const& event)
	{
		if (!CanDropChessPiece())
		{
			//TODO: error handling
			return mvc::ModelAndView::Invalid();
		}

		GetDependency<PlayerService>().DropChessPiece();
		return CreateChessboardModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::OnChessPieceMovedEvent(ChessPieceMovedEvent const& event)
	{
		if (!CanMoveChessPieceToPosition(event.PieceId, event.NewPosition))
		{
			//TODO: error handling
			return mvc::ModelAndView::Invalid();
		}

		GetDependency<BoardService>().MoveChessPieceToPosition(event.PieceId, event.NewPosition);
		GetDependency<PlayerService>().OnChessPieceMovedToPosition(event.PieceId, event.NewPosition);

		return CreateChessboardModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::OnPawnPromotedEvent(PawnPromotedEvent const& event)
	{
		if (!CanPromotePawn(event.PawnId, event.PromotedToPiece))
		{
			//TODO: error handling
			return mvc::ModelAndView::Invalid();
		}

		GetDependency<BoardService>().PromotePawn(event.PawnId, event.PromotedToPiece);
		GetDependency<PlayerService>().OnPawnPromoted();

		return CreateChessboardModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::CreateChessboardModelAndView() const
	{
		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(ViewTypeToId(ViewType::Chessboard));
		modelAndView.SetModel(STRING_ID("playerTurn"), CreateChessGameViewModel());
		return modelAndView;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::Model> ChessGameController::CreateChessGameViewModel() const
	{
		auto& playerService = GetDependency<PlayerService>();
		auto& boardService = GetDependency<BoardService>();

		auto model = std::make_unique<ChessGameViewModel>();
		model->TurnState = playerService.GetTurnState();
		model->ActivePlayerColor = playerService.GetActivePlayerColor();
		model->ChessBoard = ModelMapper::ProduceChessBoardView(boardService.GetBoardState(), playerService.GetPickedPiece());
		model->PickedPieceId = playerService.GetPickedPiece();

		if (model->PickedPieceId.IsValid())
		{
			const auto& possibleMoves = boardService.GetChessPiecePossibleMoves(model->PickedPieceId);
			std::transform(possibleMoves.begin(), possibleMoves.end(), std::back_inserter(model->PossibleMoves), ModelMapper::MapTilePositionView);
		}

		return model;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanPickChessPiece(ChessPieceId chessPieceId) const
	{
		return GetDependency<PlayerService>().CanPickChessPiece(chessPieceId)
			&& GetDependency<BoardService>().IsChessPieceOnBoard(chessPieceId);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanDropChessPiece() const
	{
		return GetDependency<PlayerService>().CanDropChessPiece();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const
	{
		auto& playerService = GetDependency<PlayerService>();
		auto& boardService = GetDependency<BoardService>();

		return playerService.GetPickedPiece() == chessPieceId
			&& boardService.IsChessPieceOnBoard(chessPieceId)
			&& boardService.CanMoveChessPieceToPosition(chessPieceId, position);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanPromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const
	{
		return GetDependency<PlayerService>().GetPickedPiece() == pawnId
			&& GetDependency<BoardService>().CanPromotePawn(pawnId, promotedToPiece);
	}
}