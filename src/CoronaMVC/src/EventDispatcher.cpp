#include "pch.h"
#include "EventDispatcher.h"

#include "mvc/BaseController.h"
#include "event/Event.h"
#include "ViewResolver.h"

#include "SystemEventTypes.h"
#include "exception/UnhandledEventException.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::AddController(EventType eventType, const BaseController* controller)
	{
		LogReturnIf(!controller && "Null controller added for event type {0}", VOID_RETURN, eventType);
		m_controllers[eventType].push_back(controller);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::DispatchEvent(const Event& event)
	{
		CheckApplicationStopRequested(event);

		auto it = m_controllers.find(event.GetType());
		if (it == m_controllers.end())
		{
			throw UnhandledEventExcpetion(event);
		}

		for (auto controller : it->second)
		{
			auto modelAndView = controller->ConsumeEvent(event);
			if (modelAndView.IsValid())
			{
				m_modelAndViewResponses.push_back(std::move(modelAndView));
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::ProcessEventResponses(ViewResolver& viewResolver)
	{
		for (auto& modelAndView : m_modelAndViewResponses)
		{
			viewResolver.UpdateView(std::move(modelAndView));
		}

		m_modelAndViewResponses.clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool EventDispatcher::IsApplicationStopRequested() const
	{
		return m_applicationStopRequested;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::CheckApplicationStopRequested(const Event& event)
	{
		static constexpr EventType STOP_EVENT_TYPE = event_types::ESystemEventType::ApplicationStopRequested;
		m_applicationStopRequested = event.GetType() == STOP_EVENT_TYPE;
	}
}