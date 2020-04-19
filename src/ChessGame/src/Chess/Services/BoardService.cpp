#include "stdafx.h"
#include "BoardService.h"
#include "ChessPieces/ChessPieceRegistry.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	BoardService::BoardService()
	{
		InitializePieces();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::vector<ChessPieceId> BoardService::GetBoardState() const
	{
		std::vector<ChessPieceId> board(CHESS_BOARD_SIDE*CHESS_BOARD_SIDE, ChessPieceId::Invalid());
		for (const auto& [pieceId, tilePosition] : m_pieces)
		{
			if (tilePosition.IsValid())
			{
				board[tilePosition.AsIndex()] = pieceId;
			}
		}

		return board;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::list<TilePosition> BoardService::GetPossibleMovesForChessPiece(ChessPieceId pieceId) const
	{
		return std::list<TilePosition>(); //TODO: implement
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool BoardService::CanMoveChessPieceToPosition(ChessPieceId chessPieceId, const TilePosition& position) const
	{
		return true; //TODO: implement
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool BoardService::IsChessPieceOnBoard(ChessPieceId chessPieceId) const
	{
		auto it = m_pieces.find(chessPieceId);
		return it != m_pieces.end() && it->second.IsValid();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::MoveChessPieceToPosition(ChessPieceId chessPiece, const TilePosition& position)
	{
		//TODO: implement more profound moves (castle, en passant)
		auto pieceToRemove = GetChessPieceOnPosition(position);
		m_pieces.at(chessPiece) = position;
		if (pieceToRemove.IsValid())
		{
			assert(pieceToRemove.GetColor() != chessPiece.GetColor());
			m_pieces.at(pieceToRemove) = TilePosition::Invalid();
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::InitializePieces()
	{
		auto pieceRegistry = GetDependency<ChessPieceRegistry>();
		for (int pieceType = 0; pieceType < (int)EChessPieceType::COUNT; ++pieceType)
		{
			const EChessPieceType type = (EChessPieceType)pieceType;
			const std::size_t instancesCount = pieceRegistry->GetInstancesCount(type);
			for (int instace = 0; instace < instancesCount; ++instace)
			{
				m_pieces.emplace(ChessPieceId(type, EColor::White, instace), pieceRegistry->GetInitialPosition(type, EColor::White, instace));
				m_pieces.emplace(ChessPieceId(type, EColor::Black, instace), pieceRegistry->GetInitialPosition(type, EColor::Black, instace));
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceId BoardService::GetChessPieceOnPosition(const TilePosition& position) const
	{
		for (const auto& [pieceId, piecePosition] : m_pieces)
		{
			if (piecePosition == position)
			{
				return pieceId;
			}
		}

		return ChessPieceId::Invalid();
	}
}