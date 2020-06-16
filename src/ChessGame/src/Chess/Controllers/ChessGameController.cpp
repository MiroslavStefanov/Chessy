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
#include "Models/GameOverViewModel.h"
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
		return CreateModelAndView();
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
		return CreateModelAndView();
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
		return CreateModelAndView();
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

		UpdateCheckState();
		return CreateModelAndView();
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

		UpdateCheckState();
		return CreateModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::CreateModelAndView() const
	{
		if (m_gameOver)
		{
			mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(ViewTypeToId(ViewType::GameOver));
			modelAndView.SetModel(CreateGameOverViewModel());
			return modelAndView;
		}

		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(ViewTypeToId(ViewType::Chessboard));
		modelAndView.SetModel(CreateChessGameViewModel());
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
		model->ActivePlayerInCheck = playerService.IsActivePlayerInCheck();

		if (model->PickedPieceId.IsValid())
		{
			const auto& possibleMoves = boardService.GetChessPiecePossibleMoves(model->PickedPieceId, playerService.IsActivePlayerInCheck());
			std::transform(possibleMoves.begin(), possibleMoves.end(), std::back_inserter(model->PossibleMoves), ModelMapper::MapTilePositionView);
		}

		return model;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::Model> ChessGameController::CreateGameOverViewModel() const
	{
		auto& playerService = GetDependency<PlayerService>();
		auto& boardService = GetDependency<BoardService>();

		auto model = std::make_unique<GameOverViewModel>();
		model->ChessBoard = ModelMapper::ProduceChessBoardView(boardService.GetBoardState(), playerService.GetPickedPiece());
		model->WinnerColor = playerService.GetActivePlayerColor();
		
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
			&& boardService.CanMoveChessPieceToPosition(chessPieceId, position, playerService.IsActivePlayerInCheck());
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanPromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const
	{
		return GetDependency<PlayerService>().GetPickedPiece() == pawnId
			&& GetDependency<BoardService>().CanPromotePawn(pawnId, promotedToPiece);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessGameController::UpdateCheckState()
	{
		auto& playerService = GetDependency<PlayerService>();
		const auto activePlayerColor = playerService.GetActivePlayerColor();

		playerService.SetActivePlayerCheckState(GetDependency<BoardService>().GetPlayerCheckState(activePlayerColor));
		m_gameOver = playerService.GetTurnState() == ETurnState::GameOver;
	}
}