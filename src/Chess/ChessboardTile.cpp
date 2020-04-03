#include "ChessboardTile.h"

#include "ChessPieces/ChessPiece.h"
#include <algorithm>

namespace chess
{
	ChessboardTile::ChessboardTile(const TilePosition& position, EColor color) :
		m_position(position), m_color(color)
	{
	}

	const TilePosition& ChessboardTile::GetTilePosition() const
	{
		return m_position;
	}

	void ChessboardTile::SetChessPiece(ChessPiece* piece, bool isInit)
	{
		m_chessPiece = piece;
		if (m_chessPiece)
		{
			m_chessPiece->SetOwnerTilePosition(m_position, isInit);
		}
	}

	const ChessPiece* ChessboardTile::GetChessPiece() const
	{
		return m_chessPiece;
	}

	ChessPiece* ChessboardTile::GetMutableChessPiece() const
	{
		return m_chessPiece;
	}

	const std::vector<TileHit>& ChessboardTile::GetHits() const
	{
		return m_hits;
	}

	bool ChessboardTile::IsCheckTile() const
	{
		return m_checkTile;
	}

	void ChessboardTile::AddHit(const TileHit& hit)
	{
		m_hits.push_back(hit);
	}

	void ChessboardTile::RemoveHitter(const ChessPiece* piece)
	{
		auto it = std::find_if(m_hits.begin(), m_hits.end(), [&piece](const TileHit& tileHit) {
			return tileHit.hitter == piece;
		});
		if (it != m_hits.end())
		{
			m_hits.erase(it);
		}
		else
		{
			//Error
		}
	}

	void ChessboardTile::ToggleCheckTile()
	{
		m_checkTile = !m_checkTile;
	}

	std::string ChessboardTile::GetDrawString()
	{
		if (!m_chessPiece)
		{
			return "0";
		}
		else
		{
			return m_chessPiece->GetDrawString();
		}
	}
}