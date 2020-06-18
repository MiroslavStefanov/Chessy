#include "pch.h"
#include "ViewResolver.h"

#include "mvc/ModelAndView.h"
#include "mvc/View.h"
#include "io/InputDevice.h"
#include "io/OutputDevice.h"

#include "exception/ViewNotFoundException.h"

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
		LogReturnIf(!viewId.IsValid() && "Trying to add view invalid id", VOID_RETURN);
		LogReturnIf(!view && "Trying to add null view with id {0}", VOID_RETURN, (int)viewId);

		m_views[viewId] = std::move(view);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::UpdateView(ModelAndView modelAndView)
	{
		auto& view = GetViewById(modelAndView.GetViewId());

		m_activeViewId = modelAndView.GetViewId();
		if (modelAndView.HasModel())
		{
			view.SetModel(modelAndView.ReleaseModel());
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::InputActiveView(InputDevice* inputDevice) const
	{
		auto& view = GetViewById(m_activeViewId);
		view.ProcessInput(inputDevice);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ViewResolver::RenderActiveView(OutputDevice* outputDevice) const
	{
		auto& view = GetViewById(m_activeViewId);
		view.ProcessOutput(outputDevice);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ViewId ViewResolver::GetActiveViewId() const
	{
		return m_activeViewId;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	View& ViewResolver::GetViewById(ViewId id) const
	{
		auto it = m_views.find(id);
		if (it == m_views.end())
		{
			throw ViewNotFoundException(id);
		}

		assert(it->second);
		return *it->second;
	}

}