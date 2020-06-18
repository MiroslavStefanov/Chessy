#include "pch.h"
#include "EventDispatcher.h"

#include "mvc/BaseController.h"
#include "event/Event.h"
#include "ViewResolver.h"

#include "SystemEventTypes.h"
#include "system_defaults/error_view/ErrorView.h"
#include "exception/UnhandledEventException.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::SetControllerExceptionHandler(ControllerExceptionHandler errorHandler)
	{
		m_errorHandler = std::move(errorHandler);
	}

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
		if (HasErrors())
		{
			return;
		}

		auto it = m_controllers.find(event.GetType());
		if (it == m_controllers.end())
		{
			throw UnhandledEventExcpetion(event);
		}

		for (auto controller : it->second)
		{
			try
			{
				auto modelAndView = controller->ConsumeEvent(event);
				if (modelAndView.IsValid())
				{
					m_modelAndViewResponses.push_back(std::move(modelAndView));
				}
			}
			catch (std::exception & e)
			{
				HandleControllerException(e);
				break;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::ProcessEventResponses(ViewResolver& viewResolver)
	{
		if (HasErrors())
		{
			InjectErrorViewModel(viewResolver.GetActiveViewId());
		}

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

	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::HandleControllerException(const std::exception& e)
	{
		m_lastError = e.what();
		m_modelAndViewResponses.clear();

		assert(m_errorHandler);
		m_modelAndViewResponses.push_back(m_errorHandler(e));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool EventDispatcher::HasErrors() const
	{
		return !m_lastError.empty();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::InjectErrorViewModel(ViewId activeViewId)
	{
		assert(m_modelAndViewResponses.size() == 1);
		auto& errorModelAndView = m_modelAndViewResponses.front();
		if (errorModelAndView.HasModel()) //there already is an error view model
		{
			m_lastError.clear();
			return;
		}

		auto errorViewModel = std::make_unique<ErrorViewModel>();
		errorViewModel->LastViewId = activeViewId;
		errorViewModel->Error = std::move(m_lastError);
		m_lastError.clear();

		errorModelAndView.SetModel(std::move(errorViewModel));
	}
}