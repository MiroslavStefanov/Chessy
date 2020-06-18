#pragma once
#include "mvc/BaseController.h"
#include "dependency/Depender.h"
#include "ErrorHandling/ErrorCodes.h"

namespace chess
{
	enum class ETurnState : int;
	class PlayerService;
	class BoardService;
	class TilePosition;
	class ValidationResult;

	class ChessGameController : public mvc::BaseController, private mvc::Depender<PlayerService, BoardService>
	{
	protected:
		void RegisterConsumers() override;
		mvc::ModelAndView OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event) override;

	private:
		mvc::ModelAndView OnChessPiecePickedEvent(class ChessPiecePickedEvent const& event);
		mvc::ModelAndView OnChessPieceDroppedEvent(class ChessPieceDroppedEvent const& event);
		mvc::ModelAndView OnChessPieceMovedEvent(class ChessPieceMovedEvent const& event);
		mvc::ModelAndView OnPawnPromotedEvent(class PawnPromotedEvent const& event);

		mvc::ModelAndView CreateModelAndView(ErrorCodes&& errors = ErrorCodes()) const;
		std::unique_ptr<mvc::Model> CreateChessGameViewModel(ErrorCodes&& errors) const;
		std::unique_ptr<mvc::Model> CreateGameOverViewModel() const;

	private:
		ValidationResult ValidatePickChessPiece(ChessPieceId chessPieceId) const;
		ValidationResult ValidateDropChessPiece() const;
		ValidationResult ValidateMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const;
		ValidationResult ValidatePromotePawn(ChessPieceId pawnId, EChessPieceType promotedToPiece) const;

		void UpdateCheckState();

	private:
		bool m_gameOver = false;
	};
}