#pragma once
#include "event/Event.h"
#include "SystemEventTypes.h"

namespace mvc
{
	class ApplicationStopRequestedEvent : public Event
	{
	public:
		EventType GetType() const override;
	};
}

DEFINE_EVENT_TYPE(mvc::ApplicationStopRequestedEvent, mvc::event_types::ESystemEventType::ApplicationStopRequested)