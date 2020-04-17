#include "pch.h"
#include "BaseApplication.h"
#include "EventDispatcher.h"
#include "ViewResolver.h"
#include "io/InputDevice.h"
#include "io/OutputDevice.h"
#include "mvc/BaseController.h"
#include "mvc/View.h"
#include "mvc/ModelAndView.h"
#include "event/ApplicationStartedEvent.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	BaseApplication::BaseApplication() : m_isRunning(false)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	BaseApplication::~BaseApplication() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::Initialize()
	{
		PreInitialize();

		PopulateControllers();
		RegisterControllersForEvents();

		PopulateViews();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::Start()
	{
		m_isRunning = true;

		m_eventDispatcher->DispatchEvent(ApplicationStartedEvent());

		while (m_isRunning)
		{
			SimulateFrame();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::PreInitialize()
	{
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

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::SetInputDevice(std::unique_ptr<InputDevice>&& inputDevice)
	{
		m_inputDevice = std::move(inputDevice);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::SetOutputDevice(std::unique_ptr<OutputDevice>&& outputDevice)
	{
		m_outputDevice = std::move(outputDevice);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::SimulateFrame()
	{
		m_eventDispatcher->ProcessEventResponses(*m_viewResolver);
		m_viewResolver->RenderActiveView(m_outputDevice.get());
		m_viewResolver->InputActiveView(m_inputDevice.get());

		m_outputDevice->Update();
		m_inputDevice->Update();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::RegisterControllersForEvents()
	{
		for (auto& controller : m_controllers)
		{
			controller->RegisterToDispatcher(*m_eventDispatcher);
		}
	}
}