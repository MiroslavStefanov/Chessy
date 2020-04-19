#include "stdafx.h"
#include "ChessPieceDroppedEvent.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	EventType ChessPieceDroppedEvent::GetType() const
	{
		return EventTypeInfo<ChessPieceDroppedEvent>::Type;
	}
}

