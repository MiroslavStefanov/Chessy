#pragma once

namespace mvc
{
	class Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetType() const = 0;
	};
}