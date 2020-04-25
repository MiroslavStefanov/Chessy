#pragma once
#include "Utils/Utils.h"

namespace chess
{
	class ChessPieceMovementIterator
	{
	private:
		static const std::unordered_map<EDirection, Position> STRAIGHT_MOVE_OFFSET_TABLE;
		static const std::unordered_map<EDirection, Position> KNIGHT_MOVE_OFFSET_TABLE;

	public:
		ChessPieceMovementIterator(const std::vector<ChessPieceId>& boardState, const TilePosition& initialPosition);
		virtual ~ChessPieceMovementIterator() = default;

		void Advance();
		bool IsDone() const;

		const TilePosition& GetTilePosition() const;

	protected:
		virtual void TryMove(const ChessPieceMove& move);
		TilePosition CalculateMovePosition(const ChessPieceMove& move) const;

		void StartNextGameMove();
		void SetTilePosition(const TilePosition& position);

	protected:
		const std::vector<ChessPieceId>& m_boardState;
		const TilePosition m_initialPosition;

		ChessPieceId m_chessPieceId;

	private:
		const ChessPieceMove& GetChessPieceMove() const;
		Position GetMoveOffset(const ChessPieceMove& move) const;
		void Finish();

	private:
		std::size_t m_gameMovesIndex;
		TilePosition m_currentPosition;
		TilePosition m_nextStartingPosition;
		bool m_successfulMove;
	};
}

