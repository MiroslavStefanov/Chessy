#include "stdafx.h"
#include "BoardService.h"
#include "ChessPieces/ChessPieceRegistry.h"

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	BoardService::BoardService(const ChessPieceRegistry& pieceRepository) : m_pieceRepository(pieceRepository)
	{
		InitializePiecesPositions();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::vector<ChessPieceId> BoardService::GetBoardState() const
	{
		std::vector<ChessPieceId> board(CHESS_BOARD_SIDE*CHESS_BOARD_SIDE, ChessPieceId::Invalid());
		for (auto& pieceToTile : m_pieces)
		{
			if (pieceToTile.second.IsValid())
			{
				board[pieceToTile.second.AsIndex()] = pieceToTile.first;
			}
		}

		return board;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void BoardService::InitializePiecesPositions()
	{
		for (int pieceType = 0; pieceType < (int)EChessPieceType::COUNT; ++pieceType)
		{
			const EChessPieceType type = (EChessPieceType)pieceType;
			const std::size_t instancesCount = m_pieceRepository.GetInstancesCount(type);
			for (int instace = 0; instace < instancesCount; ++instace)
			{
				m_pieces.emplace(ChessPieceId(type, EColor::White, instace), m_pieceRepository.GetInitialPosition(type, EColor::White, instace));
				m_pieces.emplace(ChessPieceId(type, EColor::Black, instace), m_pieceRepository.GetInitialPosition(type, EColor::Black, instace));
			}
		}
	}
}