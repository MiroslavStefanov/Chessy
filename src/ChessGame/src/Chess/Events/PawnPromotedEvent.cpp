#include "stdafx.h"
#include "PawnPromotedEvent.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	PawnPromotedEvent::PawnPromotedEvent(ChessPieceId pawnId, EChessPieceType promotedToPiece) : PawnId(pawnId), PromotedToPiece(promotedToPiece)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	EventType PawnPromotedEvent::GetType() const
	{
		return EventTypeInfo<PawnPromotedEvent>::Type;
	}
}

