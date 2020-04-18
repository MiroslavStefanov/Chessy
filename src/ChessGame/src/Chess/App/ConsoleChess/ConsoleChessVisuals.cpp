#include "stdafx.h"
#include "ConsoleChessVisuals.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	const VectorMap<EColor, char> ConsoleChessVisuals::TILE_COLOR_VISUAL_TABLE =
	{
		{EColor::Black		, ' '},
		{EColor::White		, '.'},
		{EColor::Colorless	, (char)227}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	const VectorMap<EColor, const char*> ConsoleChessVisuals::DEFAULT_PLAYER_NAME_TEXT_TABLE =
	{
		{EColor::Black		, "Black"},
		{EColor::White		, "White"},
		{EColor::Colorless	, "Invalid player"}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	const VectorMap<ETurnState, const char*> ConsoleChessVisuals::TURN_STATE_NAME_TEXT_TABLE =
	{
		{ETurnState::StartGame	, "StartGame"},
		{ETurnState::EndTurn	, "EndTurn"},
		{ETurnState::Select		, "Select"},
		{ETurnState::Unselect	, "Unselect"},
		{ETurnState::ErrorState	, "ErrorState"},
		{ETurnState::Checkmate	, "Checkmate"}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	int ConsoleChessVisuals::ColumnIndexFromVisual(char column)
	{
		return int(std::toupper(column) - 'A');
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	char ConsoleChessVisuals::ColumnVisualFromIndex(int column)
	{
		return char(column + 'A');
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	int ConsoleChessVisuals::RowIndexFromVisual(char row)
	{
		const int parsedRow = int(row - '0');
		return (8 - parsedRow);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	char ConsoleChessVisuals::RowVisualFromIndex(int row)
	{
		const int invertedRow = 8 - row;
		return char(invertedRow + '0');
	}

}