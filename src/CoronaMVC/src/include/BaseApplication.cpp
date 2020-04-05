#include "pch.h"
#include "BaseApplication.h"
#include "EventDispatcher.h"
#include "ViewResolver.h"
#include "io/BaseInputDevice.h"
#include "io/BaseOutputDevice.h"
#include "mvc/BaseController.h"
#include "mvc/View.h"
#include "mvc/ModelAndView.h"
#include "event/ApplicationStartedEvent.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	BaseApplication::BaseApplication(std::unique_ptr<BaseInputDevice>&& inputDevice, std::unique_ptr<BaseOutputDevice>&& outputDevice)
		: m_eventDispathcer(new EventDispatcher())
		, m_viewResolver(new ViewResolver())
		, m_inputDevice(std::move(inputDevice))
		, m_outputDevice(std::move(outputDevice))
		, m_isRunning(false)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	BaseApplication::~BaseApplication() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::Initialize()
	{
		PopulateControllers();
		for (auto& controller : m_controllers)
		{
			controller->RegisterToDispatcher(*m_eventDispathcer);
		}

		PopulateViews();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::Start()
	{
		m_isRunning = true;
		ApplicationStartedEvent startEvent;
		m_eventDispathcer->DispatchEvent(startEvent);

		while (m_isRunning)
		{
			SimulateFrame();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::SimulateFrame()
	{
		while (m_eventDispathcer->HasPendingViews())
		{
			m_viewResolver->UpdateView(m_eventDispathcer->PopNextView());
		}
		m_viewResolver->RenderActiveView(m_outputDevice.get());

		m_viewResolver->InputActiveView(m_inputDevice.get());
		m_inputDevice->VisitAndClearEvents(std::bind(&EventDispatcher::DispatchEvent, m_eventDispathcer.get(), std::placeholders::_1));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::Stop()
	{
		m_isRunning = false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::AddController(std::unique_ptr<BaseController>&& controller)
	{
		m_controllers.push_back(std::move(controller));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::AddView(ViewId viewId, std::unique_ptr<View>&& view)
	{
		m_viewResolver->AddView(viewId, std::move(view));
	}

}