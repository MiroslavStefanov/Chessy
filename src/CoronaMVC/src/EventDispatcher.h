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
		void SetControllerExceptionHandler(ControllerExceptionHandler defaultErrorHandler);

		void AddController(EventType eventType, const BaseController* controller);
		void DispatchEvent(const Event& event);

		void ProcessEventResponses(ViewResolver& viewResolver);
		bool IsApplicationStopRequested() const;

	private:
		void CheckApplicationStopRequested(const Event& event);
		void HandleControllerException(const std::exception& e);

		bool HasErrors() const;
		void InjectErrorViewModel(ViewId activeViewId);

	private:
		std::unordered_map<EventType, std::vector<const BaseController*>> m_controllers;
		std::vector<ModelAndView> m_modelAndViewResponses;

		ControllerExceptionHandler m_errorHandler;
		bool m_applicationStopRequested = false;
		std::string m_lastError;
	};
}