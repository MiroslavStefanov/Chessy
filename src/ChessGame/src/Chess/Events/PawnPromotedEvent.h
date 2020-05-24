#pragma once
#include "event/Event.h"
#include "EventTypes.h"

#include "Utils/Utils.h"

namespace chess
{
	enum class EChessPieceType;

	class PawnPromotedEvent : public mvc::Event
	{
	public:
		PawnPromotedEvent(ChessPieceId pawnId, EChessPieceType promotedToPiece);
		EventType GetType() const override;

	public:
		const ChessPieceId PawnId;
		const EChessPieceType PromotedToPiece;
	};
}

DEFINE_EVENT_TYPE(chess::PawnPromotedEvent, chess::event_types::EEventType::PawnPromoted)