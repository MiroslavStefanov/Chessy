#include "stdafx.h"
#include "ChessboardView.h"
#include "App/ConsoleChess/ConsoleChessInputDevice.h"
#include "App/ConsoleChess/ConsoleChessOutputDevice.h"
#include "Events/ChessPieceDroppedEvent.h"
#include "Events/ChessPiecePickedEvent.h"
#include "Events/ChessPieceMovedEvent.h"
#include "mvc/ModelAndView.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::SetModel(StringId modelId, std::unique_ptr<mvc::Model>&& model)
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

		auto& inputPosition = device->GetInputTilePosition();
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
		else if (m_model.PickedPieceId.IsValid())
		{
			RaiseEvent(ChessPieceMovedEvent(m_model.PickedPieceId, inputPosition));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessboardView::ProcessOutput(mvc::OutputDevice* outputDevice)
	{
		auto device = dynamic_cast<ConsoleChessOutputDevice*>(outputDevice);
		if (!device)
			return;

		device->RenderChessboard(m_model.ChessBoard);
		device->RenderActivePlayer(m_model.ActivePlayerColor);
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
}