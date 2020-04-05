#pragma once

namespace mvc
{
	class ViewResolver
	{
	public:
		ViewResolver();
		~ViewResolver();

		void AddView(ViewId viewId, std::unique_ptr<View>&& view);

		void InputActiveView(BaseInputDevice* inputDevice) const;
		void UpdateView(const ModelAndView& modelAndView);
		void RenderActiveView(BaseOutputDevice* outputDevice) const;

	private:
		std::unordered_map<ViewId, std::unique_ptr<View>, ViewIdHash> m_views;
		ViewId m_activeViewId;
	};
}