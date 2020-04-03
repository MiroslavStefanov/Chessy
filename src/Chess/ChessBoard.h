#pragma once

#include "ChessboardTile.h"

namespace chess
{
	class Chessy;
	enum EColor : int;

	class ChessBoard
	{
	public:
		ChessBoard(EChessboardSide whiteSide);
		~ChessBoard();

		void Initialize(EColor topColor, EColor bottomColor);

		const ChessboardTile* GetTileOnPosition(const TilePosition& position) const;
		ChessboardTile* GetMutableTileOnPosition(const TilePosition& position) const;
		ChessboardTile** GetTiles() const;
		const std::vector<TilePosition>& GetPositionsForRelculate() const;
		const Jump& GetPreviousJump() const;
		const std::vector<TilePosition>& GetTilesInCheck()const;

		const ChessPiece* GetPickedUp() const;
		void PickUp(const TilePosition& position);
		void Drop();

		void MovePickedUpTo(const TilePosition& position);
		void AddHit(const TilePosition& position, EMoveType hitType, const ChessPiece* hitter);
		
		void CheckTileOnPosition(const TilePosition& position);
		void ClearTilesInCheck();

		void Draw();

	private:
		void TakePosition(const TilePosition& position);
		bool PositionOutOfBorders(const TilePosition& position) const;

	private:
		ChessboardTile** m_board;
		ChessPiece* m_pickedUp;
		Jump m_previousJump;
		std::vector<TilePosition> m_positionsForRecalc;
		std::vector<TilePosition> m_tilesInCheck;
	};
}