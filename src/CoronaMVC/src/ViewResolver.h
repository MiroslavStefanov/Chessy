#pragma once

namespace mvc
{
	class ViewResolver
	{
	public:
		ViewResolver();
		~ViewResolver();

		void AddView(ViewId viewId, std::unique_ptr<View>&& view);
		void UpdateView(ModelAndView modelAndView);

		void InputActiveView(InputDevice* inputDevice) const;
		void RenderActiveView(OutputDevice* outputDevice) const;

	private:
		View& GetViewById(ViewId id) const;

	private:
		std::unordered_map<ViewId, std::unique_ptr<View>, ViewIdHash> m_views;
		ViewId m_activeViewId;
	};
}