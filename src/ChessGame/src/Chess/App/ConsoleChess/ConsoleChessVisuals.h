#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class ConsoleChessVisuals
	{
	public:
		//Tiles visualization
		static constexpr char PICKED_TILE_VISUAL = '+';

		//General visualization
		static constexpr char EMPTY_OFFSET_VISUAL = ' ';
		static constexpr char VERTICAL_BORDER_VISUAL = '|';
		static constexpr char HORIZONTAL_BORDER_VISUAL = '-';

		//Visual sizes
		static constexpr int CELL_SYMBOL_HEIGHT = 3;
		static constexpr int CELL_SYMBOL_WIDTH = 5;

		static constexpr auto ACTIVE_PLAYER_TEXT = "Player to play: ";
		static constexpr auto PICKED_PIECE_TEXT = "Picked chess piece: ";
		static constexpr auto POSSIBLE_MOVES_TEXT = "Can Move To: ";

		static const VectorMap<EColor, char> TILE_COLOR_VISUAL_TABLE;
		static const VectorMap<EColor, const char*> DEFAULT_PLAYER_NAME_TEXT_TABLE;
		static const VectorMap<ETurnState, const char*> TURN_STATE_NAME_TEXT_TABLE;

	public:
		static int ColumnIndexFromVisual(char column);
		static char ColumnVisualFromIndex(int column);

		static int RowIndexFromVisual(char row);
		static char RowVisualFromIndex(int row);
	};
}