#pragma once
#include "Utils/Utils.h"
#include "dependency/Depender.h"

namespace chess
{
	class ChessPieceRegistry;

	class BoardService : public mvc::Depender<ChessPieceRegistry>
	{
	public:
		BoardService();

		std::vector<ChessPieceId>	GetBoardState() const;
		std::list<TilePosition>		GetPossibleMovesForChessPiece(ChessPieceId pieceId) const;

		bool CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const;
		bool IsChessPieceOnBoard(ChessPieceId chessPieceId) const;

		void MoveChessPieceToPosition(ChessPieceId chessPiece, const TilePosition& position);

	private:
		void InitializePieces();
		ChessPieceId GetChessPieceOnPosition(const TilePosition& position) const;

	private:
		std::unordered_map<ChessPieceId, TilePosition, ChessPieceIdHash> m_pieces;

	};
}

