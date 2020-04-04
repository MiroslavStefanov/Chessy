#pragma once

namespace mvc
{
	class View
	{
	public:
		virtual ~View() = default;
		virtual void Update(const ModelAndView& modelAndView) = 0;
		virtual void Render(BaseOutputDevice& outputDevice) = 0;
	};
}