#pragma once
#include "mvc/BaseController.h"
#include "dependency/Depender.h"

namespace chess
{
	enum class ETurnState : int;
	class PlayerService;
	class BoardService;
	class TilePosition;

	class ChessGameController : public mvc::BaseController, private mvc::Depender<PlayerService, BoardService>
	{
	protected:
		void RegisterConsumers() override;
		mvc::ModelAndView OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event) override;

	private:
		mvc::ModelAndView OnChessPiecePickedEvent(class ChessPiecePickedEvent const& event);
		mvc::ModelAndView OnChessPieceDroppedEvent(class ChessPieceDroppedEvent const& event);
		mvc::ModelAndView OnChessPieceMovedEvent(class ChessPieceMovedEvent const& event);

		std::unique_ptr<mvc::Model> CreateChessGameViewModel() const;

	private:
		bool CanPickChessPiece(ChessPieceId chessPieceId) const;
		bool CanDropChessPiece() const;
		bool CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const;
	};
}