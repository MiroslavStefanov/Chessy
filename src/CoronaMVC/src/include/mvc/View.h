#pragma once
#include "dependency/Depender.h"

namespace mvc
{
	class View : private Depender<class EventDispatcher>
	{
	public:
		View();
		virtual ~View();
		virtual void SetModel(std::unique_ptr<Model>&& model) = 0;
		virtual void ProcessInput(InputDevice* inputDevice) = 0;
		virtual void ProcessOutput(OutputDevice* outputDevice) = 0;

	protected:
		void RaiseEvent(const Event& event);
		void StopApplication();
	};
}