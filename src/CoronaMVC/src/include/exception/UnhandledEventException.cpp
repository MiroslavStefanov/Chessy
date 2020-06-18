#include "pch.h"
#include "UnhandledEventException.h"
#include "event/Event.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	UnhandledEventExcpetion::UnhandledEventExcpetion(const Event& event) noexcept
		: LoggedException("Unhadled event: " + event.GetDescription())
	{
	}

}