#pragma once
#include "Utils/Utils.h"

namespace chess
{
	struct ChessPiece
	{
		std::size_t InstacesCount;
		char Visualization;
		std::vector<TilePosition> WhiteInitialPositions;
		std::vector<TilePosition> BlackInitialPositions;
		std::vector<ChessPieceMove> RelativeMoves;
	};

	//class ChessboardTile;
	//class TilePosition;
	//struct ChessPieceMove;
	//enum class EColor : int;
	//enum class EChessPieceType : int;
	//
	//class ChessPiece
	//{
	//public:
	//	ChessPiece(EChessPieceType type);

	//	//game moves
	//	void SetPossibleGameMoves(const std::vector<TilePosition>& possibleGameMoves);
	//	const std::vector<TilePosition>& GetPossibleGameMoves() const;
	//	bool IsPossibleMove(const TilePosition& position) const;

	//	//owner tile
	//	void SetOwnerTilePosition(const TilePosition& ownerTilePosition, bool isInit);
	//	const TilePosition& GetOnwerTilePosition() const;

	//	//getters
	//	const std::vector<ChessPieceMove>& GetChessPieceMoves() const;
	//	EChessPieceType GetType() const;
	//	bool HasMoved() const;

	//	std::string GetDrawString();

	//protected:
	//	std::vector<ChessPieceMove> m_chessMoves;

	//private:
	//	bool					m_hasMoved;
	//	EChessPieceType			m_type;
	//	TilePosition				m_ownerTilePosition;
	//	std::vector<TilePosition>	m_possibleMoves;
	//};
}    
