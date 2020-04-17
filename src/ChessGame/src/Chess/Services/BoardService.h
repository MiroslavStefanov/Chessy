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

		std::vector<ChessPieceId> GetBoardState() const;

		void MoveChessPiece(ChessPieceId chessPiece, const TilePosition& newPosition);

	private:
		void InitializePieces();

	private:
		std::unordered_map<ChessPieceId, TilePosition, ChessPieceIdHash> m_pieces;

	};
}

