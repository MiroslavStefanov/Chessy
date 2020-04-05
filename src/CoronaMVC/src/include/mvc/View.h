#pragma once

namespace mvc
{
	class View
	{
	public:
		virtual ~View() = default;
		virtual void Update(const ModelAndView& modelAndView) = 0;
		virtual void ProcessInput(BaseInputDevice* inputDevice) = 0;
		virtual void ProcessOutput(BaseOutputDevice* outputDevice) = 0;
	};
}