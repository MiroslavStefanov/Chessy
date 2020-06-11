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
		auto playerService = GetDependency<PlayerService>();
		playerService->StartGame();

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

		auto playerService = GetDependency<PlayerService>();
		playerService->PickChessPiece(event.PieceId);

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

		auto playerService = GetDependency<PlayerService>();
		playerService->DropChessPiece();

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

		auto boardService = GetDependency<BoardService>();
		boardService->MoveChessPieceToPosition(event.PieceId, event.NewPosition);

		auto playerService = GetDependency<PlayerService>();
		playerService->OnChessPieceMovedToPosition(event.PieceId, event.NewPosition);

		UpdateCheckState();
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

		auto boardService = GetDependency<BoardService>();
		boardService->PromotePawn(event.PawnId, event.PromotedToPiece);

		auto playerService = GetDependency<PlayerService>();
		playerService->OnPawnPromoted();

		UpdateCheckState();
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
		auto playerService = GetDependency<PlayerService>();
		auto boardService = GetDependency<BoardService>();

		auto model = std::make_unique<ChessGameViewModel>();
		model->TurnState = playerService->GetTurnState();
		model->ActivePlayerColor = playerService->GetActivePlayerColor();
		model->ChessBoard = ModelMapper::ProduceChessBoardView(boardService->GetBoardState(), playerService->GetPickedPiece());
		model->PickedPieceId = playerService->GetPickedPiece();

		if (model->PickedPieceId.IsValid())
		{
			const auto& possibleMoves = boardService->GetChessPiecePossibleMoves(model->PickedPieceId);
			std::transform(possibleMoves.begin(), possibleMoves.end(), std::back_inserter(model->PossibleMoves), ModelMapper::MapTilePositionView);
		}

		return model;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanPickChessPiece(ChessPieceId chessPieceId) const
	{
		return GetDependency<PlayerService>()->CanPickChessPiece(chessPieceId)
			&& GetDependency<BoardService>()->IsChessPieceOnBoard(chessPieceId);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanDropChessPiece() const
	{
		return GetDependency<PlayerService>()->CanDropChessPiece();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const
	{
		return GetDependency<PlayerService>()->GetPickedPiece() == chessPieceId
			&& GetDependency<BoardService>()->IsChessPieceOnBoard(chessPieceId)
			&& GetDependency<BoardService>()->CanMoveChessPieceToPosition(chessPieceId, position);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ChessGameController::CanPromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const
	{
		return GetDependency<PlayerService>()->GetPickedPiece() == pawnId
			&& GetDependency<BoardService>()->CanPromotePawn(pawnId, promotedToPiece);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessGameController::UpdateCheckState()
	{
		auto playerService = GetDependency<PlayerService>();
		const auto activePlayerColor = playerService->GetActivePlayerColor();

		auto playerCheckStateResolver = GetDependency<BoardService>()->CreatePlayerCheckStateResolver(activePlayerColor);
		playerService->SetActivePlayerCheckState(playerCheckStateResolver.ResolvePlayerCheckState());
	}
}