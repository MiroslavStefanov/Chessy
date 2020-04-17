#include "stdafx.h"
#include "ConsoleChessOutputDevice.h"
#include "Models/ChessboardViewModel.h"
#include "Models/PlayerTurnViewModel.h"
#include "ChessPieces/ChessPieceRegistry.h"

namespace chess
{
	static constexpr auto PICKED_TILE_CHAR = '+';
	static constexpr auto WHITE_TILE_CHAR = '.';
	static constexpr auto BLACK_TILE_CHAR = ' ';
	static constexpr char INVALID_TILE_CHAR = (char)227;

	////////////////////////////////////////////////////////////////////////////////////////////////
	char GetColorChar(EColor color)
	{
		switch (color)
		{
		case chess::EColor::White:
			return WHITE_TILE_CHAR;
		case chess::EColor::Black:
			return BLACK_TILE_CHAR;
		default:
			return INVALID_TILE_CHAR;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::Update()
	{
		ClearConsole();
		std::cout << m_frameBuffer.str();
		m_frameBuffer.clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderChessboard(const ChessboardViewModel& chessboard)
	{
		const size_t boardTiles = CHESS_BOARD_SIDE * CHESS_BOARD_SIDE;
		if (boardTiles != chessboard.Tiles.size())
		{
			assert(false);
			return;
		}

		for (int i = 0; i < CHESS_BOARD_SIDE; ++i)
		{
			const auto rowIndexOffset = i * CHESS_BOARD_SIDE;
			auto rowBegin = chessboard.Tiles.cbegin() + rowIndexOffset;
			auto rowEnd = rowBegin + CHESS_BOARD_SIDE;
			RenderChessboardRow(rowBegin, rowEnd);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderPlayerTurn(const PlayerTurnViewModel& playerTurn)
	{
		if (!playerTurn.PossibleMoves.empty())
		{
			m_frameBuffer << "Can Move To:";
			for (auto& position : playerTurn.PossibleMoves)
			{
				m_frameBuffer << position.Row << "," << position.Column << ";";
			}
		}
		m_frameBuffer << std::endl;

		RenderTurnState(playerTurn.TurnState);
		m_frameBuffer << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::ClearConsole()
	{
		system("CLS");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderChessTile(const ChessTileViewModel& tile)
	{
		auto pieceRegistry = GetDependency<ChessPieceRegistry>();
		char tileChar = INVALID_TILE_CHAR;
		if (tile.IsPicked)
		{
			tileChar = PICKED_TILE_CHAR;
		}
		else if (tile.Piece.IsValid() && pieceRegistry)
		{
			tileChar = pieceRegistry->GetVisualRepresentation(tile.Piece.GetType(), tile.Piece.GetColor());
		}
		else
		{
			tileChar = GetColorChar(tile.Color);
		}

		m_frameBuffer << tileChar;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderTurnState(ETurnState turnState)
	{
		switch (turnState)
		{
		case chess::ETurnState::EndTurn:
			m_frameBuffer << "EndTurn";
			break;
		case chess::ETurnState::Select:
			m_frameBuffer << "Select";
			break;
		case chess::ETurnState::Unselect:
			m_frameBuffer << "Unselect";
			break;
		case chess::ETurnState::ErrorState:
			m_frameBuffer << "ErrorState";
			break;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderChessboardRow(std::vector<ChessTileViewModel>::const_iterator rowBegin, std::vector<ChessTileViewModel>::const_iterator rowEnd)
	{
		static constexpr int CELL_SYMBOL_SIZE = 3;

		for (int row = 0; row < CELL_SYMBOL_SIZE; ++row)
		{
			for (auto tileIt = rowBegin; tileIt != rowEnd; ++tileIt)
			{
				for (int column = 0; column < CELL_SYMBOL_SIZE; ++column)
				{
					const bool shouldDrawBackground = (row == 0 || row == CELL_SYMBOL_SIZE - 1) || (column == 0 || column == CELL_SYMBOL_SIZE - 1);
					if (shouldDrawBackground)
					{
						m_frameBuffer << GetColorChar(tileIt->Color);
					}
					else
					{
						RenderChessTile(*tileIt);
					}
				}
			}
			m_frameBuffer << std::endl;
		}
	}
}