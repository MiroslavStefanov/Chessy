#pragma once
#include "mvc/ModelAndView.h"

namespace mvc
{
	class BaseController;
	class Event;
	class ViewResolver;

	class EventDispatcher
	{
	public:
		void AddController(EventType eventType, const BaseController* controller);
		void DispatchEvent(const Event& event);

		void ProcessEventResponses(ViewResolver& viewResolver);
	private:
		std::unordered_map<EventType, std::vector<const BaseController*>> m_controllers;
		std::vector<ModelAndView> m_modelAndViewResponses;
	};
}