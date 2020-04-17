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
	void BoardService::MoveChessPiece(ChessPieceId chessPiece, const TilePosition& newPosition)
	{
		//TODO: implement
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
}