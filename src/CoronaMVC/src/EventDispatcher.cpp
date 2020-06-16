#include "pch.h"
#include "EventDispatcher.h"

#include "mvc/BaseController.h"
#include "event/Event.h"
#include "ViewResolver.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::AddController(EventType eventType, const BaseController* controller)
	{
		if (!controller)
		{
			assert(false);
			return;
		}

		const auto [iterator, success] = m_controllers.emplace(eventType, controller);
		assert(success && "There already is a controller for this event type");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::DispatchEvent(const Event& event)
	{
		auto it = m_controllers.find(event.GetType());
		if (it == m_controllers.end())
		{
			assert(false);
			return;
		}

		auto modelAndView = it->second->ConsumeEvent(event);
		if (modelAndView.IsValid())
		{
			m_modelAndViewResponses.push_back(std::move(modelAndView));
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
}