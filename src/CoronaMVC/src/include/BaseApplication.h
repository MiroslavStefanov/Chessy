#pragma once
#include "dependency/DependencyWrapper.h"

namespace mvc
{
	class BaseApplication
	{
	public:
		BaseApplication();
		virtual ~BaseApplication();

		void Initialize();
		void Start();

	protected:
		virtual void PreInitialize();
		virtual void PopulateControllers() = 0;
		virtual void PopulateViews() = 0;

		void AddController(std::unique_ptr<BaseController>&& controller);
		void AddView(ViewId viewId, std::unique_ptr<View>&& view);

		void SetInputDevice(std::unique_ptr<InputDevice>&& inputDevice);
		void SetOutputDevice(std::unique_ptr<OutputDevice>&& outputDevice);

		void SetLogFileName(const std::string& fileName);

	private:
		void SimulateFrame();
		void RegisterControllersForEvents();

		std::string GetDefaultLogFileName();

	private:
		DependencyWrapper<class EventDispatcher> m_eventDispatcher;
		DependencyWrapper<class ViewResolver> m_viewResolver;

		std::unique_ptr<InputDevice> m_inputDevice;
		std::unique_ptr<OutputDevice> m_outputDevice;

		std::vector<std::unique_ptr<BaseController>> m_controllers;
	};
}