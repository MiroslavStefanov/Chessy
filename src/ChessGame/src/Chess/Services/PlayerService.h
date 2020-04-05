#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class PlayerService
	{
	public:
		PlayerService(class ChessPieceRegistry const& pieceRepository);

		std::vector<TilePosition> GetPossibleMoves() const;
		bool IsPicked(ChessPieceId pieceId) const;

		std::vector<std::unique_ptr<mvc::Event>> GenerateEventsForTile(const TilePosition& tilePosition, const ChessPieceId& tilePiece) const;

	private:
		const ChessPieceRegistry& m_pieceRepository;
		std::unordered_map<EColor, ChessPieceId> m_pickedPieces;
		EColor m_activePlayerColor;
	};
}

