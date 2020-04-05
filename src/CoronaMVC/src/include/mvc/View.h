#pragma once

namespace mvc
{
	class View
	{
	public:
		virtual ~View() = default;
		virtual void Update(ModelAndView&& modelAndView) = 0;
		virtual void ProcessInput(BaseInputDevice* inputDevice) = 0;
		virtual void ProcessOutput(BaseOutputDevice* outputDevice) = 0;
	};
}