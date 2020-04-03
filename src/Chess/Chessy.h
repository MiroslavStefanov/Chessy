#pragma once

#include "Utils/Utils.h"

namespace chess
{
	class TilePosition;
	class ChessBoard;
	class ChessboardTile;
	struct Position;
	struct ChessySide;
	enum EColor : int;
	enum EChessboardSide : int;
	enum ETurnState : int;

	class Chessy
	{
	public:
		Chessy(EChessboardSide whiteSide = EChessboardSide::Bottom);

		ETurnState ChessyTurn(const TilePosition& position);

		void Draw();

		static void OnEndTurn();
	protected:
		struct ChessySide
		{
			ChessySide(EChessboardSide side, EColor color)
				: Side(side),
				Color(color),
				CastlePositions{ 
				TilePosition(Position(Side == Bottom ? 7 : 0, GetKingPosition(Color))),
				TilePosition(Position(Side == Bottom ? 7 : 0, 0)),
				TilePosition(Position(Side == Bottom ? 7 : 0, 7)) }
			{
			}

			void AddChessPiece(const ChessPiece* chessPiece)
			{
				ChessPieces.push_back(chessPiece);
			}

			EChessboardSide Side;
			EColor Color;
			TilePosition CastlePositions[3];
			std::vector<const ChessPiece*> ChessPieces;
		};

	private:
		bool IsPossibleMove(const TilePosition& position);
		void CastleCheck();
		void CheckmateCheck();
		void RecalculateGameMoves(const TilePosition& position);
		void CalculateGameMoves(const TilePosition& position);
		Position CalculateModifier(const ChessPieceMove& direction, const ChessPiece* pieceColor);
		void RecalculateGameMovesForBlocked(const TilePosition& position);
		void HandleKing(const TilePosition& position, const ChessPieceMove& chessPieceMove);
		void HandlePawn(const TilePosition& position, const ChessPieceMove& chessPieceMove);
		void HandleChessPiece(const TilePosition& position, const ChessPieceMove& move);

		bool m_checkmate;
		bool m_check;
		bool m_activateCheck;
		static EColor m_turnColor;
		ChessySide m_topSide;
		ChessySide m_bottomSide;
		ChessBoard* m_chessBoard;

		//helper
		std::vector<TilePosition> m_possibleGameMoves;
	};
}

