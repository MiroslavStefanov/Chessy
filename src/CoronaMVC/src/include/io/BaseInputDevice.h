#pragma once
#include "BufferQueue.h"

namespace mvc
{
	class BaseInputDevice 
	{
	public:
		BaseInputDevice();
		virtual ~BaseInputDevice();

		void VisitAndClearEvents(std::function<void(Event&)> eventVisitor);
		void AddEvent(std::unique_ptr<Event>&& event);

	private:
		BufferQueue<std::unique_ptr<Event>> m_events;
	};
}