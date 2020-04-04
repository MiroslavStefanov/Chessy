#pragma once
#include "event/Event.h"
#include "EventTypes.h"

#include "Utils/Utils.h"

namespace chess
{
	class CellClickedEvent : public mvc::Event
	{
	public:
		CellClickedEvent(const TilePosition& position);
		EventType GetType() const override;

	public:
		const TilePosition Position;
	};
}

DEFINE_EVENT_TYPE(chess::CellClickedEvent, chess::event_types::EEventType::CellClicked)