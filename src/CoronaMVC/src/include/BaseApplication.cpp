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
#include "logger/FileLoggerImpl.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	BaseApplication::BaseApplication() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////
	BaseApplication::~BaseApplication() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::Initialize()
	{
		SetLogFileName(GetDefaultLogFileName());

		PreInitialize();
		assert(m_inputDevice);
		assert(m_outputDevice);

		PopulateControllers();
		RegisterControllersForEvents();

		PopulateViews();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::Start()
	{
		m_eventDispatcher->DispatchEvent(ApplicationStartedEvent());
		while(!m_eventDispatcher->IsApplicationStopRequested())
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
	void BaseApplication::SetLogFileName(const std::string& fileName)
	{
		Logger::Get().SetLoggerImplementation(std::make_unique<FileLoggerImpl>(fileName));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::SimulateFrame()
	{
		m_eventDispatcher->ProcessEventResponses(*m_viewResolver);
		m_viewResolver->RenderActiveView(m_outputDevice.get());
		m_outputDevice->Update();
		m_inputDevice->Update();
		m_viewResolver->InputActiveView(m_inputDevice.get());
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void BaseApplication::RegisterControllersForEvents()
	{
		for (auto& controller : m_controllers)
		{
			controller->RegisterToDispatcher(*m_eventDispatcher);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string BaseApplication::GetDefaultLogFileName()
	{
		static constexpr auto DEFAULT_LOG_FILE_NAME_PREFIX = "Log_";

		const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		return std::string(DEFAULT_LOG_FILE_NAME_PREFIX) + std::to_string(now);
	}
}