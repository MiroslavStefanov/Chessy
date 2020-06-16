#include "pch.h"
#include "ApplicationStopRequestedEvent.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	EventType ApplicationStopRequestedEvent::GetType() const
	{
		return EventTypeInfo<ApplicationStopRequestedEvent>::Type;
	}
}

