#pragma once
#include "mvc/View.h"
#include "mvc/Model.h"

namespace mvc
{
	class ViewResolver;

	struct ErrorViewModel : public Model
	{
		ViewId LastViewId = ViewId::Invalid();
		std::string Error;
	};

	class DefaultErrorView : public View
	{
	public:
		static const ViewId DEFAULT_ERROR_VIEW_ID;

	public:
		DefaultErrorView(ViewResolver& viewResolver);

		void SetModel(std::unique_ptr<mvc::Model>&& model) override;
		void ProcessInput(mvc::InputDevice* inputDevice) override;
		void ProcessOutput(mvc::OutputDevice* outputDevice) override;

	private:
		ViewResolver& m_viewResolver;
		ErrorViewModel m_model;
	};
}