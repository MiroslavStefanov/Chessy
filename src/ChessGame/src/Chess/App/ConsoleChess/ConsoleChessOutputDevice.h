#pragma once
#include "io/OutputDevice.h"
#include "dependency/Depender.h"
#include "ChessPieces/ChessPieceRegistry.h"

#include <sstream>

namespace chess
{
	enum class ETurnState;
	enum class EColor;
	class ChessPieceRegistry;
	struct ChessTileViewModel;
	struct TilePositionViewModel;

	class ConsoleChessOutputDevice : public mvc::OutputDevice, public mvc::Depender<ChessPieceRegistry>
	{
	public:
		void Update() override;

		void RenderChessboard(const std::vector<ChessTileViewModel>& chessBoardTiles);
		void RenderPossibleMoves(const std::vector<TilePositionViewModel>& possibleMoves);
		void RenderPickedChessPiece(ChessPieceId chessPieceId);
		void RenderActivePlayer(EColor activePlayerColor);
		void RenderTurnState(ETurnState turnState);

	private:
		void ClearConsole();

		void RenderChessBoardRow(std::vector<ChessTileViewModel>::const_iterator rowBegin, int rowNumber);
		void RenderBorderRow();
		void RenderLettersRow();

		char GetTileCenterVisual(const ChessTileViewModel& tile) const;

	private:
		std::stringstream m_frameBuffer;
	};
}