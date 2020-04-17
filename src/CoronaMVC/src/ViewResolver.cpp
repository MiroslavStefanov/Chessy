#include "pch.h"
#include "ViewResolver.h"

#include "mvc/ModelAndView.h"
#include "mvc/View.h"
#include "io/InputDevice.h"
#include "io/OutputDevice.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ViewResolver::ViewResolver() : m_activeViewId(ViewId::Invalid())
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ViewResolver::~ViewResolver() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::AddView(ViewId viewId, std::unique_ptr<View>&& view)
	{
		if (!viewId.IsValid() || !view)
		{
			assert(false);
			return;
		}

		m_views[viewId] = std::move(view);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::UpdateView(ModelAndView modelAndView)
	{
		m_activeViewId = modelAndView.GetViewId();
		auto it = m_views.find(m_activeViewId);
		if (it != m_views.end() && modelAndView.HasModel())
		{
			it->second->SetModel(modelAndView.GetModelId(), modelAndView.ReleaseModel());
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::InputActiveView(InputDevice* inputDevice) const
	{
		auto it = m_views.find(m_activeViewId);
		if (it != m_views.end())
		{
			it->second->ProcessInput(inputDevice);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::RenderActiveView(OutputDevice* outputDevice) const
	{
		auto it = m_views.find(m_activeViewId);
		if (it != m_views.end())
		{
			it->second->ProcessOutput(outputDevice);
		}
	}

}