#pragma once

#include "Utils/Utils.h"
#include <string>

namespace chess
{
	class ChessPiece;
	class TilePosition;
	struct TileHit;
	enum class EColor : int;

	class ChessboardTile
	{
	public:
		ChessboardTile(const TilePosition& position, EColor color);

		const TilePosition& GetTilePosition() const;
		const ChessPiece* GetChessPiece() const;
		ChessPiece* GetMutableChessPiece() const;
		const std::vector<TileHit>& GetHits() const;
		bool IsCheckTile() const;

		void AddHit(const TileHit& hit);
		void RemoveHitter(const ChessPiece* piece);
		void ToggleCheckTile();
		void SetChessPiece(ChessPiece* piece, bool isInit = false);

		std::string GetDrawString();

	private:
		bool m_checkTile;
		TilePosition m_position;
		EColor m_color;
		ChessPiece* m_chessPiece;
		std::vector<TileHit> m_hits;
	};
}
