#include "pch.h"
#include "ApplicationStopRequestedEvent.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	EventType ApplicationStopRequestedEvent::GetType() const
	{
		return EventTypeInfo<ApplicationStopRequestedEvent>::Type;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string ApplicationStopRequestedEvent::GetDescription() const
	{
		return "Application stop was requested";
	}
}

