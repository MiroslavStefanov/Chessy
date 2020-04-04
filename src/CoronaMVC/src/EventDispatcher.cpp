#include "pch.h"
#include "EventDispatcher.h"

#include "mvc/BaseController.h"
#include "mvc/ModelAndView.h"
#include "event/Event.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::AddController(EventType eventType, const BaseController* controller)
	{
		m_controllers[eventType].push_back(controller);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void EventDispatcher::DispatchEvent(const Event& event)
	{
		auto it = m_controllers.find(event.GetType());
		if (it == m_controllers.end())
			return;

		for (auto controller : it->second)
		{
			auto modelAndView = controller->ConsumeEvent(event);
			if (modelAndView.IsValid())
			{
				m_eventResponses.AddEntry(std::move(modelAndView));
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////
	bool EventDispatcher::HasPendingViews() const
	{
		return !m_eventResponses.IsEmpty();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ModelAndView EventDispatcher::PopNextView()
	{
		if (!HasPendingViews())
			return ModelAndView::Invalid();

		return m_eventResponses.PopNextEntry();
	}
}