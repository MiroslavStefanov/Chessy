#include "pch.h"
#include "ViewResolver.h"

#include "mvc/ModelAndView.h"
#include "mvc/View.h"

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
	void ViewResolver::UpdateView(const ModelAndView& modelAndView)
	{
		auto it = m_views.find(modelAndView.GetViewId());
		if (it != m_views.end() && it->second)
		{
			it->second->Update(modelAndView);
		}

		m_activeViewId = modelAndView.GetViewId();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::RenderActiveView(BaseOutputDevice& outputDevice) const
	{
		auto it = m_views.find(m_activeViewId);
		if (it != m_views.end() && it->second)
		{
			it->second->Render(outputDevice);
		}
	}

}