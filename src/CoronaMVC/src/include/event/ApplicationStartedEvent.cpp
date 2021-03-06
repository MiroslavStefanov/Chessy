#include "pch.h"
#include "ApplicationStartedEvent.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	EventType ApplicationStartedEvent::GetType() const
	{
		return EventTypeInfo<ApplicationStartedEvent>::Type;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string ApplicationStartedEvent::GetDescription() const
	{
		return "Application started";
	}
}

