#include "pch.h"
#include "ErrorView.h"
#include "io/OutputDevice.h"
#include "io/InputDevice.h"
#include "ViewResolver.h"
#include "mvc/ModelAndView.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	const ViewId DefaultErrorView::DEFAULT_ERROR_VIEW_ID = ViewId(-99999999);

	////////////////////////////////////////////////////////////////////////////////////////////////
	DefaultErrorView::DefaultErrorView(ViewResolver& viewResolver)
		: m_viewResolver(viewResolver)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void DefaultErrorView::SetModel(std::unique_ptr<mvc::Model>&& model)
	{
		std::unique_ptr<mvc::Model> localModel = std::move(model);
		auto errorViewModel = dynamic_cast<ErrorViewModel*>(localModel.get());
		LogReturnIf(!errorViewModel, VOID_RETURN);

		m_model = std::move(*errorViewModel);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void DefaultErrorView::ProcessInput(mvc::InputDevice* inputDevice)
	{
		assert(inputDevice);
		if (inputDevice->HasAnyInput())
		{
			m_viewResolver.UpdateView(ModelAndView::CreateFromViewId(m_model.LastViewId));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void DefaultErrorView::ProcessOutput(mvc::OutputDevice* outputDevice)
	{
		assert(outputDevice);
		if (!m_model.Error.empty())
		{
			outputDevice->RenderText(m_model.Error);
		}
	}
}
