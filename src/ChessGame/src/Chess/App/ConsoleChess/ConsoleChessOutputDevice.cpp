#include "stdafx.h"
#include "ConsoleChessOutputDevice.h"
#include "Models/ChessboardViewModel.h"
#include "Models/PlayerTurnViewModel.h"
#include "ChessPieces/ChessPieceRegistry.h"
#include "ConsoleChessVisuals.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::Update()
	{
		ClearConsole();
		std::cout << m_frameBuffer.str();
		std::stringstream().swap(m_frameBuffer);
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

		RenderLettersRow();
		RenderBorderRow();
		for (int i = 0; i < CHESS_BOARD_SIDE; ++i)
		{
			const auto rowIndexOffset = i * CHESS_BOARD_SIDE;
			auto rowBegin = chessboard.Tiles.cbegin() + rowIndexOffset;
			RenderChessboardRow(rowBegin, i);
		}
		RenderBorderRow();
		RenderLettersRow();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderPlayerTurn(const PlayerTurnViewModel& playerTurn)
	{
		m_frameBuffer 
			<< ConsoleChessVisuals::ACTIVE_PLAYER_TEXT
			<< ConsoleChessVisuals::DEFAULT_PLAYER_NAME_TEXT_TABLE[playerTurn.ActivePlayerColor] 
			<< std::endl;

		if (playerTurn.PickedPieceId.IsValid())
		{
			auto pieceRegistry = GetDependency<ChessPieceRegistry>();
			m_frameBuffer 
				<< ConsoleChessVisuals::PICKED_PIECE_TEXT 
				<< pieceRegistry->GetVisualRepresentation(playerTurn.PickedPieceId.GetType(), playerTurn.PickedPieceId.GetColor())
				<< std::endl;
		}

		if (!playerTurn.PossibleMoves.empty())
		{
			const auto possibleMovesSize = playerTurn.PossibleMoves.size();
			m_frameBuffer << ConsoleChessVisuals::POSSIBLE_MOVES_TEXT;
			for (std::size_t i = 0; i < possibleMovesSize; ++i)
			{
				const auto& position = playerTurn.PossibleMoves[i];
				m_frameBuffer << ConsoleChessVisuals::ColumnVisualFromIndex(position.Column) << ConsoleChessVisuals::RowVisualFromIndex(position.Row);
				if (i < possibleMovesSize - 1)
				{
					m_frameBuffer << ", ";
				}
			}
			m_frameBuffer << std::endl;
		}

		m_frameBuffer << ConsoleChessVisuals::TURN_STATE_NAME_TEXT_TABLE[playerTurn.TurnState] << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::ClearConsole()
	{
		system("CLS");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderChessboardRow(std::vector<ChessTileViewModel>::const_iterator rowBegin, int rowNumber)
	{
		const auto rowEnd = rowBegin + CHESS_BOARD_SIDE;
		for (int row = 0; row < ConsoleChessVisuals::CELL_SYMBOL_HEIGHT; ++row)
		{
			const bool shoudRenderRowNumber = row == ConsoleChessVisuals::CELL_SYMBOL_HEIGHT / 2;
			const char rowNumberSymbol = shoudRenderRowNumber ? ConsoleChessVisuals::RowVisualFromIndex(rowNumber) : ConsoleChessVisuals::EMPTY_OFFSET_VISUAL;

			m_frameBuffer << rowNumberSymbol;
			m_frameBuffer << ConsoleChessVisuals::VERTICAL_BORDER_VISUAL;

			for (auto tileIt = rowBegin; tileIt != rowEnd; ++tileIt)
			{
				for (int column = 0; column < ConsoleChessVisuals::CELL_SYMBOL_WIDTH; ++column)
				{
					const bool shouldRenderBackground = row != ConsoleChessVisuals::CELL_SYMBOL_HEIGHT/2 || column != ConsoleChessVisuals::CELL_SYMBOL_WIDTH/2;
					m_frameBuffer << (shouldRenderBackground ? ConsoleChessVisuals::TILE_COLOR_VISUAL_TABLE[tileIt->Color] : GetTileCenterVisual(*tileIt));
				}
			}

			m_frameBuffer << ConsoleChessVisuals::VERTICAL_BORDER_VISUAL;
			m_frameBuffer << rowNumberSymbol;
			m_frameBuffer << std::endl;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderBorderRow()
	{
		m_frameBuffer << ConsoleChessVisuals::EMPTY_OFFSET_VISUAL;

		for (int i = 0; i < ConsoleChessVisuals::CELL_SYMBOL_WIDTH * CHESS_BOARD_SIDE + 2; ++i)
		{
			m_frameBuffer << ConsoleChessVisuals::HORIZONTAL_BORDER_VISUAL;
		}

		m_frameBuffer << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ConsoleChessOutputDevice::RenderLettersRow()
	{
		m_frameBuffer << ConsoleChessVisuals::EMPTY_OFFSET_VISUAL << ConsoleChessVisuals::EMPTY_OFFSET_VISUAL;

		for (int column = 0; column < CHESS_BOARD_SIDE; ++column)
		{
			for (int symbolIndex = 0; symbolIndex < ConsoleChessVisuals::CELL_SYMBOL_WIDTH; ++symbolIndex)
			{
				const bool shouldRenderLetter = symbolIndex == ConsoleChessVisuals::CELL_SYMBOL_WIDTH / 2;
				m_frameBuffer << (shouldRenderLetter ? ConsoleChessVisuals::ColumnVisualFromIndex(column) : ConsoleChessVisuals::EMPTY_OFFSET_VISUAL);
			}
		}

		m_frameBuffer << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	char ConsoleChessOutputDevice::GetTileCenterVisual(const ChessTileViewModel& tile) const
	{
		char result = ConsoleChessVisuals::TILE_COLOR_VISUAL_TABLE[tile.Color];

		if (tile.IsPicked)
		{
			result = ConsoleChessVisuals::PICKED_TILE_VISUAL;
		}
		else if (tile.Piece.IsValid())
		{
			auto pieceRegistry = GetDependency<ChessPieceRegistry>();
			result = pieceRegistry->GetVisualRepresentation(tile.Piece.GetType(), tile.Piece.GetColor());
		}

		return result;
	}
}