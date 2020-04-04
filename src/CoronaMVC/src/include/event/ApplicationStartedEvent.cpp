#include "pch.h"
#include "ApplicationStartedEvent.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	EventType ApplicationStartedEvent::GetType() const
	{
		return EventTypeInfo<ApplicationStartedEvent>::Type;
	}
}

