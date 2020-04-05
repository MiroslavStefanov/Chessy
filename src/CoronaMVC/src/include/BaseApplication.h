#pragma once

namespace mvc
{
	class BaseApplication
	{
	public:
		BaseApplication(std::unique_ptr<BaseInputDevice>&& inputDevice, std::unique_ptr<BaseOutputDevice>&& outputDevice);
		virtual ~BaseApplication();

		void Initialize();
		void Start();

	protected:
		virtual void PreInitialize();
		virtual void PopulateControllers() = 0;
		virtual void PopulateViews() = 0;

		void Stop();
		void AddController(std::unique_ptr<BaseController>&& controller);
		void AddView(ViewId viewId, std::unique_ptr<View>&& view);

	private:
		void SimulateFrame();

	private:
		std::unique_ptr<class EventDispatcher> m_eventDispathcer;
		std::unique_ptr<class ViewResolver> m_viewResolver;
		std::unique_ptr<BaseInputDevice> m_inputDevice;
		std::unique_ptr<BaseOutputDevice> m_outputDevice;
		std::vector<std::unique_ptr<BaseController>> m_controllers;

		bool m_isRunning;
	};
}