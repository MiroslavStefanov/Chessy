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

#include "ErrorHandling/ValidationResult.h"

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
		auto validation = ValidatePickChessPiece(event.PieceId);
		if (!validation.IsValid())
		{
			return CreateModelAndView(std::move(validation.PopErrors()));
		}

		GetDependency<PlayerService>().PickChessPiece(event.PieceId);
		return CreateModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::OnChessPieceDroppedEvent(ChessPieceDroppedEvent const& event)
	{
		auto validation = ValidateDropChessPiece();
		if (!validation.IsValid())
		{
			return CreateModelAndView(std::move(validation.PopErrors()));
		}

		GetDependency<PlayerService>().DropChessPiece();
		return CreateModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::OnChessPieceMovedEvent(ChessPieceMovedEvent const& event)
	{
		auto validation = ValidateMoveChessPieceToPosition(event.PieceId, event.NewPosition);
		if (!validation.IsValid())
		{
			return CreateModelAndView(std::move(validation.PopErrors()));
		}

		GetDependency<BoardService>().MoveChessPieceToPosition(event.PieceId, event.NewPosition);
		GetDependency<PlayerService>().OnChessPieceMovedToPosition(event.PieceId, event.NewPosition);

		UpdateCheckState();
		return CreateModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::OnPawnPromotedEvent(PawnPromotedEvent const& event)
	{
		auto validation = ValidatePromotePawn(event.PawnId, event.PromotedToPiece);
		if (!validation.IsValid())
		{
			return CreateModelAndView(std::move(validation.PopErrors()));
		}

		GetDependency<BoardService>().PromotePawn(event.PawnId, event.PromotedToPiece);
		GetDependency<PlayerService>().OnPawnPromoted();

		UpdateCheckState();
		return CreateModelAndView();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ModelAndView ChessGameController::CreateModelAndView(ErrorCodes&& errors) const
	{
		if (m_gameOver)
		{
			mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(ViewTypeToId(ViewType::GameOver));
			modelAndView.SetModel(CreateGameOverViewModel());
			return modelAndView;
		}

		mvc::ModelAndView modelAndView = mvc::ModelAndView::CreateFromViewId(ViewTypeToId(ViewType::Chessboard));
		modelAndView.SetModel(CreateChessGameViewModel(std::move(errors)));
		return modelAndView;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<mvc::Model> ChessGameController::CreateChessGameViewModel(ErrorCodes&& errors) const
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

		model->Errors = std::move(errors);
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
	ValidationResult ChessGameController::ValidatePickChessPiece(ChessPieceId chessPieceId) const
	{
		ValidationResult result;

		if (!GetDependency<PlayerService>().CanPickChessPiece(chessPieceId))
		{
			result.AddError(ErrorCode::CannotPickChessPiece);
		}

		if (!GetDependency<BoardService>().IsChessPieceOnBoard(chessPieceId))
		{
			result.AddError(ErrorCode::ChessPieceNotOnBoard);
		}
		
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ValidationResult ChessGameController::ValidateDropChessPiece() const
	{
		ValidationResult result;

		if (!GetDependency<PlayerService>().CanDropChessPiece())
		{
			result.AddError(ErrorCode::CannotDropChessPiece);
		}

		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ValidationResult ChessGameController::ValidateMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const
	{
		auto& playerService = GetDependency<PlayerService>();
		auto& boardService = GetDependency<BoardService>();

		ValidationResult result;

		if (playerService.GetPickedPiece() != chessPieceId)
		{
			result.AddError(ErrorCode::InvalidPickedChessPiece);
		}

		if (!boardService.IsChessPieceOnBoard(chessPieceId))
		{
			result.AddError(ErrorCode::ChessPieceNotOnBoard);
		}

		try
		{
			if (!boardService.CanMoveChessPieceToPosition(chessPieceId, position, playerService.IsActivePlayerInCheck()))
			{
				result.AddError(ErrorCode::InvalidChessPieceMove);
			}
		}
		catch (...)
		{
			result.AddError(ErrorCode::InternalError);
		}

		return result;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	ValidationResult ChessGameController::ValidatePromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const
	{
		ValidationResult result;

		if (GetDependency<PlayerService>().GetPickedPiece() != pawnId
			|| !GetDependency<BoardService>().CanPromotePawn(pawnId, promotedToPiece))
		{
			result.AddError(ErrorCode::CannotPromotePawn);
		}

		return result;
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