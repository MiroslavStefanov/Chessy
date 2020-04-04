#pragma once

namespace mvc
{
	namespace event_types
	{
		static constexpr int SYSTEM_EVENT_TYPES_COUNT = -99999999;
		enum ESystemEventType : int
		{
			ApplicationStarted = SYSTEM_EVENT_TYPES_COUNT
		};
	}
}
