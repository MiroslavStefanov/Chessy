#pragma once
#include "io/OutputDevice.h"
#include "ErrorHandling/ErrorCodes.h"

namespace chess
{
	enum class ETurnState;
	enum class EColor;
	struct ChessTileViewModel;
	struct TilePositionViewModel;

	class ConsoleChessOutputDevice : public mvc::OutputDevice
	{
	public:
		void Update() override;
		void RenderText(const std::string& text) override;

		void RenderChessboard(const std::vector<ChessTileViewModel>& chessBoardTiles);
		void RenderPossibleMoves(const std::vector<TilePositionViewModel>& possibleMoves);
		void RenderPickedChessPiece(ChessPieceId chessPieceId);
		void RenderActivePlayer(EColor activePlayerColor, bool isInCheck);
		void RenderTurnState(ETurnState turnState);
		void RenderGameOverMessage(EColor winnerColor);
		void RenderErrors(const ErrorCodes& errors);

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