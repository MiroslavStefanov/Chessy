#pragma once
#include "event/Event.h"
#include "SystemEventTypes.h"

namespace mvc
{
	class ApplicationStartedEvent : public Event
	{
	public:
		EventType GetType() const override;
	};
}

DEFINE_EVENT_TYPE(mvc::ApplicationStartedEvent, mvc::event_types::ESystemEventType::ApplicationStarted)