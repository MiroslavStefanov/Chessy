#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class EnPassantCache
	{
	public:
		bool IsValid() const;
		const TilePosition& GetEnPassantPosition() const;
		const TilePosition& GetPawnPosition() const;
		void Set(const TilePosition& enPassantPosition, const TilePosition& pawnPosition);
		void Reset();

	private:
		TilePosition m_enPassantPosition = TilePosition::Invalid();
		TilePosition m_pawnPosition = TilePosition::Invalid();
	};
}

