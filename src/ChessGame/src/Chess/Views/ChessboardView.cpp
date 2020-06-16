#include "stdafx.h"
#include "ChessboardView.h"
#include "App/ConsoleChess/ConsoleChessInputDevice.h"
#include "App/ConsoleChess/ConsoleChessOutputDevice.h"
#include "ChessPieces/ChessPieceTypes.h" 
#include "Events/ChessPieceDroppedEvent.h"
#include "Events/ChessPiecePickedEvent.h"
#include "Events/ChessPieceMovedEvent.h"
#include "Events/PawnPromotedEvent.h"
#include "mvc/ModelAndView.h"
#include "Utils/Utils.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::SetModel(std::unique_ptr<mvc::Model>&& model)
	{
		std::unique_ptr<mvc::Model> localModel = std::move(model);
		auto chessGameViewModel = dynamic_cast<ChessGameViewModel*>(localModel.get());
		if (!chessGameViewModel)
		{
			assert(false);
			return;
		}

		m_model = std::move(*chessGameViewModel);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessInput(mvc::InputDevice* inputDevice)
	{
		auto device = dynamic_cast<ConsoleChessInputDevice*>(inputDevice);
		if (!device)
			return;

		auto& inputPosition = device->GetTilePosition();
		auto inputChessPieceType = device->GetChessPieceType();

		switch (m_model.TurnState)
		{
		case ETurnState::StartGame:
		case ETurnState::EndTurn:
		case ETurnState::Unselect:
			PickChessPiceOnPosition(inputPosition);
			break;
		case ETurnState::Select:
			PlaySelectedPieceOnPosition(inputPosition);
			break;
		case ETurnState::PawnPromotion:
			PromotePawnToChessPieceType(inputChessPieceType);
			break;
		default:
			break;
		}

		if (!inputPosition.IsValid())
		{
			return;
		}

		const auto& inputTile = m_model.ChessBoard[inputPosition.AsIndex()];
		if (inputTile.IsPicked)
		{
			RaiseEvent(ChessPieceDroppedEvent());
		}
		else if (!m_model.PickedPieceId.IsValid() && inputTile.Piece.IsValid() && inputTile.Piece.GetColor() == m_model.ActivePlayerColor)
		{
			RaiseEvent(ChessPiecePickedEvent(inputTile.Piece));
		}
		else
		{
			const bool isPossibleMove = std::any_of(
				m_model.PossibleMoves.cbegin(),
				m_model.PossibleMoves.cend(),
				[&inputPosition](const TilePositionViewModel& position) -> bool
				{
					return Position(position.Row, position.Column) == inputPosition.AsPosition();
				}
			);

			if (isPossibleMove)
			{
				RaiseEvent(ChessPieceMovedEvent(m_model.PickedPieceId, inputPosition));
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessOutput(mvc::OutputDevice* outputDevice)
	{
		auto device = dynamic_cast<ConsoleChessOutputDevice*>(outputDevice);
		if (!device)
			return;

		device->RenderChessboard(m_model.ChessBoard);
		device->RenderActivePlayer(m_model.ActivePlayerColor, m_model.ActivePlayerInCheck);
		if (m_model.PickedPieceId.IsValid())
		{
			device->RenderPickedChessPiece(m_model.PickedPieceId);
		}
		if (!m_model.PossibleMoves.empty())
		{
			device->RenderPossibleMoves(m_model.PossibleMoves);
		}
		device->RenderTurnState(m_model.TurnState);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::PickChessPiceOnPosition(const TilePosition& position)
	{
		assert(position.IsValid());
		const auto& inputTile = m_model.ChessBoard[position.AsIndex()];

		const bool canPickChessPiece = !m_model.PickedPieceId.IsValid()
			&& inputTile.Piece.IsValid()
			&& inputTile.Piece.GetColor() == m_model.ActivePlayerColor;
		assert(canPickChessPiece);

		RaiseEvent(ChessPiecePickedEvent(inputTile.Piece));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::PlaySelectedPieceOnPosition(const TilePosition& position)
	{
		assert(position.IsValid());
		assert(m_model.PickedPieceId.IsValid() && m_model.PickedPieceId.GetColor() == m_model.ActivePlayerColor);

		const auto& inputTile = m_model.ChessBoard[position.AsIndex()];
		const bool isDeselect = inputTile.IsPicked;
		const bool isValidMove = std::any_of(
			m_model.PossibleMoves.cbegin(),
			m_model.PossibleMoves.cend(),
			[&position](const TilePositionViewModel& movePosition) -> bool
			{
				return Position(movePosition.Row, movePosition.Column) == position.AsPosition();
			}
		);

		if (isDeselect)
		{
			RaiseEvent(ChessPieceDroppedEvent());
		}
		else if (isValidMove)
		{
			RaiseEvent(ChessPieceMovedEvent(m_model.PickedPieceId, position));
		}
		else
		{
			assert(false);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::PromotePawnToChessPieceType(EChessPieceType chessPieceType)
	{
		assert(chessPieceType != EChessPieceType::COUNT);

		const bool pickedPawn = m_model.PickedPieceId.IsValid()
			&& m_model.PickedPieceId.GetType() == EChessPieceType::Pawn;
		assert(pickedPawn);

		auto pawnPositionIt = std::find_if(m_model.ChessBoard.cbegin(), m_model.ChessBoard.cend(), [](const ChessTileViewModel& tile) {
			return tile.IsPicked;
			});
		assert(pawnPositionIt != m_model.ChessBoard.cend());

		const TilePosition pawnPosition = TilePosition(Position(pawnPositionIt - m_model.ChessBoard.cbegin()));
		assert(pawnPosition.IsValid());

		const bool canPromotePawn = m_model.PickedPieceId.GetColor() == m_model.ActivePlayerColor
			&& IsPawnPromotionPosition(m_model.PickedPieceId.GetColor(), pawnPosition);
		assert(canPromotePawn);

		RaiseEvent(PawnPromotedEvent(m_model.PickedPieceId, chessPieceType));
	}
}