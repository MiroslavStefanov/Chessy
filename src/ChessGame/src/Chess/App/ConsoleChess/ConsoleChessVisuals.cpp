#include "stdafx.h"
#include "ConsoleChessVisuals.h"
#include "ChessPieces/ChessPieceTypes.h"
#include "ErrorHandling/ErrorCodes.h"

namespace chess
{
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

	////////////////////////////////////////////////////////////////////////////////////////////////
	char ConsoleChessVisuals::GetChessPieceVisual(EChessPieceType chessPieceType, EColor chessPieceColor)
	{
		char visual = CHESS_PIECE_VISUAL_TABLE[chessPieceType];

		switch (chessPieceColor)
		{
		case chess::EColor::White:
			visual = std::toupper(visual);
			break;
		case chess::EColor::Black:
			visual = std::tolower(visual);
			break;
		default:
			visual = INVALID_VISUAL;
			break;
		}
		
		return visual;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const VectorMap<EColor, char> ConsoleChessVisuals::TILE_COLOR_VISUAL_TABLE =
	{
		{EColor::Black		, ' '},
		{EColor::White		, '.'},
		{EColor::Colorless	, INVALID_VISUAL}
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
		{ETurnState::StartGame, "StartGame"},
		{ETurnState::EndTurn, "EndTurn"},
		{ETurnState::Select, "Select"},
		{ETurnState::Unselect, "Unselect"},
		{ETurnState::ErrorState, "ErrorState"},
		{ETurnState::GameOver, "GameOver"},
		{ETurnState::PawnPromotion, "PawnPromotion"}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	const VectorMap<ErrorCode, const char*> ConsoleChessVisuals::ERROR_CODE_TEXT_TABLE =
	{
		{ErrorCode::InvalidTile, "Not a valid tile!"},
		{ErrorCode::CannotPickChessPiece, "Cannot pick that chess piece right now!"},
		{ErrorCode::CannotDropChessPiece, "Cannot drop the picked chess piece!"},
		{ErrorCode::InvalidPickedChessPiece, "The picked chess piece is not valid!"},
		{ErrorCode::InvalidChessPieceMove, "Cannot perform that move!"},
		{ErrorCode::CannotPromotePawn, "Cannot promote that pawn to that chess piece!"},
		{ErrorCode::ChessPieceNotOnBoard, "That chess piece is not on the board!"},
		{ErrorCode::InternalError, "Internal error!"}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	const VectorMap<EChessPieceType, char> ConsoleChessVisuals::CHESS_PIECE_VISUAL_TABLE =
	{
		{ EChessPieceType::Bishop,	'b' },
		{ EChessPieceType::King,	'w' },
		{ EChessPieceType::Knight,	'k' },
		{ EChessPieceType::Pawn,	'p' },
		{ EChessPieceType::Queen,	'q' },
		{ EChessPieceType::Rook,	'r' },
		{ EChessPieceType::COUNT,	INVALID_VISUAL }
	};
}