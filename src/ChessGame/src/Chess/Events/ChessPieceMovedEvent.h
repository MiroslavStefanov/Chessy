#pragma once
#include "event/Event.h"
#include "EventTypes.h"

#include "Utils/Utils.h"

namespace chess
{
	class ChessPieceMovedEvent : public mvc::Event
	{
	public:
		ChessPieceMovedEvent(ChessPieceId pieceId, const TilePosition& newPosition);
		EventType GetType() const override;

	public:
		const ChessPieceId PieceId;
		const TilePosition NewPosition;
	};
}

DEFINE_EVENT_TYPE(chess::ChessPieceMovedEvent, chess::event_types::EEventType::ChessPieceMoved)