#include "stdafx.h"
#include "ConsoleChessOutputDevice.h"
#include "Models/ChessboardViewModel.h"
#include "Models/PlayerTurnViewModel.h"
#include "ChessPieces/ChessPieceRegistry.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::ClearConsole()
	{
		system("CLS");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderChessboard(const ChessboardViewModel& chessboard)
	{
		const size_t boardTiles = CHESS_BOARD_SIDE * CHESS_BOARD_SIDE;
		for (std::size_t i = 0; i < boardTiles; ++i)
		{
			auto& tile = chessboard.Tiles[i];
			RenderChessTile(tile);
			if (i % CHESS_BOARD_SIDE == CHESS_BOARD_SIDE - 1)
			{
				std::cout << std::endl;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderPlayerTurn(const PlayerTurnViewModel& playerTurn)
	{
		if (!playerTurn.PossibleMoves.empty())
		{
			std::cout << "Can Move To:";
			for (auto& position : playerTurn.PossibleMoves)
			{
				std::cout << position.Row << "," << position.Column << ";";
			}
		}
		std::cout << std::endl;

		RenderTurnState(playerTurn.TurnState);
		std::cout << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::SetChessPieceRegistry(const ChessPieceRegistry* registry)
	{
		m_pieceRegistry = registry;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderChessTile(const ChessTileViewModel& tile)
	{
		static constexpr auto pickedTileChar = ' ';
		static constexpr auto whiteTileChar = '=';
		static constexpr auto blackTileChar = '#';
		static constexpr char invalidTileChar = (char)227;

		char tileChar = invalidTileChar;
		if (tile.IsPicked)
		{
			tileChar = pickedTileChar;
		}
		else if (tile.Piece.IsValid() && m_pieceRegistry)
		{
			tileChar = m_pieceRegistry->GetVisualRepresentation(tile.Piece.GetType(), tile.Piece.GetColor());
		}
		else if (tile.Color == EColor::White)
		{
			tileChar = whiteTileChar;
		}
		else if (tile.Color == EColor::Black)
		{
			tileChar = blackTileChar;
		}

		std::cout << tileChar;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderTurnState(ETurnState turnState)
	{
		switch (turnState)
		{
		case chess::ETurnState::EndTurn:
			std::cout<<"EndTurn";
			break;
		case chess::ETurnState::Select:
			std::cout << "Select";
			break;
		case chess::ETurnState::Unselect:
			std::cout << "Unselect";
			break;
		case chess::ETurnState::ErrorState:
			std::cout << "ErrorState";
			break;
		}
	}
}