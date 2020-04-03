#pragma once

#include <vector>
#include <string>
#include "../Utils/Utils.h"

namespace chess
{
	class ChessboardTile;
	class TilePosition;
	struct ChessPieceMove;
	enum EColor : int;
	enum EChessPieceType : int;
	
	class ChessPiece
	{
	public:
		ChessPiece(EChessPieceType type, EColor color);

		//game moves
		void SetPossibleGameMoves(const std::vector<TilePosition>& possibleGameMoves);
		const std::vector<TilePosition>& GetPossibleGameMoves() const;

		//owner tile
		void SetOwnerTilePosition(const TilePosition& ownerTilePosition, bool isInit);
		const TilePosition& GetOnwerTilePosition() const;

		//getters
		const std::vector<ChessPieceMove>& GetChessPieceMoves() const;
		EChessPieceType GetType() const;
		EColor GetColor() const;
		bool HasMoved() const;

		std::string GetDrawString();

	protected:
		std::vector<ChessPieceMove> m_chessMoves;

	private:
		bool					m_hasMoved;
		EColor					m_color;
		EChessPieceType			m_type;
		TilePosition				m_ownerTilePosition;
		std::vector<TilePosition>	m_possibleMoves;
	};
}    
