#pragma once

namespace mvc
{
	class Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetType() const = 0;
		virtual std::string GetDescription() const { return "Event of type " + GetType(); }
	};
}