#pragma once
#include "event/Event.h"
#include "EventTypes.h"

namespace chess
{
	class ChessPiecePickedEvent : public mvc::Event
	{
	public:
		ChessPiecePickedEvent(ChessPieceId pieceId);
		EventType GetType() const override;

	public:
		const ChessPieceId PieceId;
	};
}

DEFINE_EVENT_TYPE(chess::ChessPiecePickedEvent, chess::event_types::EEventType::ChessPiecePicked)