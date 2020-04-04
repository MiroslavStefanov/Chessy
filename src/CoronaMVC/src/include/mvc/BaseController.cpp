#include "pch.h"
#include "BaseController.h"
#include "EventDispatcher.h"
#include "mvc/ModelAndView.h"
#include "event/Event.h"
#include "event/ApplicationStartedEvent.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	BaseController::~BaseController() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseController::RegisterToDispatcher(EventDispatcher& dispatcher)
	{
		RegisterConsumers();
		for (auto& it : m_eventConsumers)
		{
			dispatcher.AddController(it.first, this);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ModelAndView BaseController::ConsumeEvent(const Event& event) const
	{
		auto it = m_eventConsumers.find(event.GetType());
		if (it != m_eventConsumers.end() && it->second)
			return it->second(event);

		return ModelAndView::Invalid();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseController::RegisterConsumers()
	{
		RegisterConsumer<ApplicationStartedEvent>(std::bind(&BaseController::OnApplicationStartedEvent, this, std::placeholders::_1));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ModelAndView BaseController::OnApplicationStartedEvent(ApplicationStartedEvent const& event)
	{
		return ModelAndView::Invalid();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseController::RegisterConsumer(EventType eventType, std::function<ModelAndView(const Event&)> consumer)
	{
		m_eventConsumers[eventType] = consumer;
	}

}