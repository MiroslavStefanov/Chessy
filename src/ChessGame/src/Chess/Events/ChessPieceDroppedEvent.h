#pragma once
#include "event/Event.h"
#include "EventTypes.h"

namespace chess
{
	class ChessPieceDroppedEvent : public mvc::Event
	{
	public:
		ChessPieceDroppedEvent() = default;
		EventType GetType() const override;
	};
}

DEFINE_EVENT_TYPE(chess::ChessPieceDroppedEvent, chess::event_types::EEventType::ChessPieceDropped)