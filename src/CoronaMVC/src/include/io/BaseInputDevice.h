#pragma once
#include "BufferQueue.h"

namespace mvc
{
	class BaseInputDevice 
	{
	public:
		BaseInputDevice();
		virtual ~BaseInputDevice();

		virtual void	CollectInputEvents() = 0;
		void			VisitAndClearEvents(std::function<void(Event&)> eventVisitor);

	protected:
		void			AddEvent(std::unique_ptr<Event>&& event);

	private:
		BufferQueue<std::unique_ptr<Event>> m_events;
	};
}