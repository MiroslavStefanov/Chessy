#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class BoardService
	{
	public:
		BoardService(class ChessPieceRegistry const& pieceRepository);

		std::vector<ChessPieceId> GetBoardState() const;

	private:
		void InitializePiecesPositions();

	private:
		const ChessPieceRegistry& m_pieceRepository;
		std::unordered_map<ChessPieceId, TilePosition, ChessPieceIdHash> m_pieces;

	};
}

