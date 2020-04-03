#include "ChessBoard.h"

#include "ChessPieces/Pawn.h"
#include "ChessPieces/Knight.h"
#include "ChessPieces/Bishop.h"
#include "ChessPieces/Rook.h"
#include "ChessPieces/Queen.h"
#include "ChessPieces/King.h"

#include "Chessy.h"

#include <iostream>

namespace chess
{
	ChessBoard::ChessBoard(EChessboardSide whiteSide) : m_pickedUp(nullptr)
	{
		m_board = new ChessboardTile*[sChessBoardSize];
		for (std::int8_t i = 0; i < sChessBoardSize; ++i)
		{
			m_board[i] = new ChessboardTile(TilePosition(i), i % 2 == 0 ? White : Black);
		}
	}

	ChessBoard::~ChessBoard()
	{
		for (std::int8_t i = 0; i < sChessBoardSize; ++i)
		{
			delete m_board[i];
		}
	}

	void ChessBoard::Initialize(EColor topColor, EColor bottomColor)
	{
		auto& board = m_board;
		auto InitializePieceOfTypeOnPosition = [&board](EChessPieceType type, const TilePosition& position, EColor color) {
			ChessPiece* piece = nullptr;
			switch (type)
			{
			case chess::PawnType:
				piece = new Pawn(color);
				break;
			case chess::KnightType:
				piece = new Knight(color);
				break;
			case chess::BishopType:
				piece = new Bishop(color);
				break;
			case chess::RookType:
				piece = new Rook(color);
				break;
			case chess::QueenType:
				piece = new Queen(color);
				break;
			case chess::KingType:
				piece = new King(color);
				break;
			}
			board[position.AsIndex()]->SetChessPiece(piece, true);
			
		};

		for (int i = 0; i < sChessBoardSide; ++i)
		{
			InitializePieceOfTypeOnPosition(PawnType, TilePosition(1 * sChessBoardSide + i), topColor);
			InitializePieceOfTypeOnPosition(PawnType, TilePosition((sChessBoardSide - 2) * sChessBoardSide + i), bottomColor);
		}

		auto lastRow = (sChessBoardSide - 1) * sChessBoardSide;
		InitializePieceOfTypeOnPosition(RookType, TilePosition(0), topColor);
		InitializePieceOfTypeOnPosition(RookType, TilePosition(sChessBoardSide - 1), topColor);
		InitializePieceOfTypeOnPosition(RookType, TilePosition(lastRow + 0), bottomColor);
		InitializePieceOfTypeOnPosition(RookType, TilePosition(lastRow + sChessBoardSide - 1), bottomColor);

		InitializePieceOfTypeOnPosition(KnightType, TilePosition(1), topColor);
		InitializePieceOfTypeOnPosition(KnightType, TilePosition(sChessBoardSide - 2), topColor);
		InitializePieceOfTypeOnPosition(KnightType, TilePosition(lastRow + 1), bottomColor);
		InitializePieceOfTypeOnPosition(KnightType, TilePosition(lastRow + sChessBoardSide - 2), bottomColor);

		InitializePieceOfTypeOnPosition(BishopType, TilePosition(2), topColor);
		InitializePieceOfTypeOnPosition(BishopType, TilePosition(sChessBoardSide - 3), topColor);
		InitializePieceOfTypeOnPosition(BishopType, TilePosition(lastRow + 2), bottomColor);
		InitializePieceOfTypeOnPosition(BishopType, TilePosition(lastRow + sChessBoardSide - 3), bottomColor);

		if (topColor == White)
		{
			InitializePieceOfTypeOnPosition(QueenType, TilePosition(4), topColor);
			InitializePieceOfTypeOnPosition(QueenType, TilePosition(lastRow + 4), bottomColor);

			InitializePieceOfTypeOnPosition(KingType, TilePosition(3), topColor);
			InitializePieceOfTypeOnPosition(KingType, TilePosition(lastRow + 3), bottomColor);
		}
		else
		{
			InitializePieceOfTypeOnPosition(QueenType, TilePosition(3), topColor);
			InitializePieceOfTypeOnPosition(QueenType, TilePosition(lastRow + 3), bottomColor);

			InitializePieceOfTypeOnPosition(KingType, TilePosition(4), topColor);
			InitializePieceOfTypeOnPosition(KingType, TilePosition(lastRow + 4), bottomColor);
		}
	}

	const ChessPiece* ChessBoard::GetPickedUp() const
	{
		return m_pickedUp;
	}

	void ChessBoard::PickUp(const TilePosition& position)
	{
		m_pickedUp = m_board[position.AsIndex()]->GetMutableChessPiece();
	}

	void ChessBoard::Drop()
	{
		m_pickedUp = nullptr;
	}

	void ChessBoard::MovePickedUpTo(const TilePosition& position)
	{
		//cleanup
		m_positionsForRecalc.clear();
		auto newPositionChessPiece = m_board[position.AsIndex()]->GetChessPiece();
		if (newPositionChessPiece != nullptr)
		{
			delete newPositionChessPiece;
		}

		//jump check
		if (position == m_previousJump.sneakyTakePosition)
		{
			TilePosition jumpPiecePosition(m_previousJump.piece->GetOnwerTilePosition());
			m_positionsForRecalc.push_back(jumpPiecePosition);
			m_board[jumpPiecePosition.AsIndex()]->SetChessPiece(nullptr);
			delete m_previousJump.piece;
		}
		m_previousJump = Jump();

		if (m_pickedUp->GetType() == PawnType)
		{
			//hack for now
			auto rowCheck1 = position.AsPosition().row == m_pickedUp->GetOnwerTilePosition().AsPosition().row + 2;
			auto rowCheck2 = position.AsPosition().row == m_pickedUp->GetOnwerTilePosition().AsPosition().row - 2;
			if (rowCheck1 || rowCheck2)
			{
				m_previousJump = Jump(rowCheck1 ?
					Position(position.AsPosition().row - 1, position.AsPosition().col) :
					Position(position.AsPosition().row + 1, position.AsPosition().col), m_pickedUp);

				m_positionsForRecalc.push_back(m_previousJump.sneakyTakePosition);
			}
		}

		//clear old tile
		auto oldPosition = m_pickedUp->GetOnwerTilePosition();
		m_board[oldPosition.AsIndex()]->SetChessPiece(nullptr);

		//positions to recalculate game moves for
		m_positionsForRecalc.push_back(position);
		m_positionsForRecalc.push_back(oldPosition);

		//move to new tile
		m_board[position.AsIndex()]->SetChessPiece(m_pickedUp);

		//change
		m_pickedUp = nullptr;
	}

	void ChessBoard::AddHit(const TilePosition& position, EMoveType moveType, const ChessPiece* hitter)
	{
		m_board[position.AsIndex()]->AddHit(TileHit(moveType, hitter));
	}

	const std::vector<TilePosition>& ChessBoard::GetTilesInCheck() const
	{
		return m_tilesInCheck;
	}

	void ChessBoard::CheckTileOnPosition(const TilePosition & position)
	{
		m_board[position.AsIndex()]->ToggleCheckTile();
		m_tilesInCheck.push_back(position);
	}

	void ChessBoard::ClearTilesInCheck()
	{
		m_tilesInCheck.clear();
	}

	const ChessboardTile* ChessBoard::GetTileOnPosition(const TilePosition& position) const
	{
		if (PositionOutOfBorders(position))
			return nullptr;
		return m_board[position.AsIndex()];
	}

	ChessboardTile * ChessBoard::GetMutableTileOnPosition(const TilePosition & position) const
	{
		if (PositionOutOfBorders(position))
			return nullptr;
		return m_board[position.AsIndex()];
	}

	ChessboardTile** ChessBoard::GetTiles() const
	{
		return m_board;
	}

	const std::vector<TilePosition>& ChessBoard::GetPositionsForRelculate() const
	{
		return m_positionsForRecalc;
	}

	const Jump & ChessBoard::GetPreviousJump() const
	{
		return m_previousJump;
	}

	void ChessBoard::Draw()
	{
		for (std::int8_t i = 0; i < sChessBoardSize; i++)
		{
			printf("%s", m_board[i]->GetDrawString().c_str());
			if (i % sChessBoardSide == sChessBoardSide - 1)
			{
				std::cout << std::endl;
			}
		}
		if (m_pickedUp)
		{
			std::cout << "Can Move To:";
			for (auto position : m_pickedUp->GetPossibleGameMoves())
			{
				std::cout << (char)('0' + position.AsPosition().row) << "," << (char)('0' + position.AsPosition().col) << ";";
			}
		}
		std::cout << std::endl;
	}

	bool ChessBoard::PositionOutOfBorders(const TilePosition& position) const// not needed with forms
	{
		return (position.AsPosition().row < 0 || position.AsPosition().row >= sChessBoardSide
			|| position.AsPosition().col < 0 || position.AsPosition().col >= sChessBoardSide);
	}
}