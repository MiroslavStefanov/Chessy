#include "stdafx.h"
#include "CellClickedEvent.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	CellClickedEvent::CellClickedEvent(const TilePosition& position) : Position(position)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	EventType CellClickedEvent::GetType() const
	{
		return EventTypeInfo<CellClickedEvent>::Type;
	}
}
