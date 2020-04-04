#pragma once
#include "mvc/ModelAndView.h"
#include "BufferQueue.h"

namespace mvc
{
	class BaseController;
	class Event;

	class EventDispatcher
	{
	public:
		void			AddController(EventType eventType, const BaseController* controller);
		void			DispatchEvent(const Event& event);
		ModelAndView	PopNextView();
		bool			HasPendingViews() const;

	private:
		std::unordered_map<EventType, std::vector<const BaseController*>> m_controllers;
		BufferQueue<ModelAndView> m_eventResponses;
	};
}