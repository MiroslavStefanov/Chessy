#include "pch.h"
#include "ViewResolver.h"

#include "mvc/ModelAndView.h"
#include "mvc/View.h"
#include "io/BaseInputDevice.h"
#include "io/BaseOutputDevice.h"

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
		if (viewId.IsValid())
		{
			m_views[viewId] = std::move(view);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::InputActiveView(BaseInputDevice* inputDevice) const
	{
		auto it = m_views.find(m_activeViewId);
		if (it != m_views.end() && it->second)
		{
			it->second->ProcessInput(inputDevice);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::UpdateView(ModelAndView&& modelAndView)
	{
		m_activeViewId = modelAndView.GetViewId();
		auto it = m_views.find(modelAndView.GetViewId());
		if (it != m_views.end() && it->second)
		{
			it->second->Update(std::move(modelAndView));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::RenderActiveView(BaseOutputDevice* outputDevice) const
	{
		auto it = m_views.find(m_activeViewId);
		if (it != m_views.end() && it->second)
		{
			it->second->ProcessOutput(outputDevice);
		}
	}

}