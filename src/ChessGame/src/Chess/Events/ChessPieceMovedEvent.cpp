#include "stdafx.h"
#include "ChessPieceMovedEvent.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceMovedEvent::ChessPieceMovedEvent(ChessPieceId pieceId, const TilePosition& newPosition) : PieceId(pieceId), NewPosition(newPosition)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	EventType ChessPieceMovedEvent::GetType() const
	{
		return EventTypeInfo<ChessPieceMovedEvent>::Type;
	}
}

