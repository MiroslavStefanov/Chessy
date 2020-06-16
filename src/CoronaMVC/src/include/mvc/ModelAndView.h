#pragma once
#include "mvc/Model.h"

namespace mvc
{
	class ModelAndView
	{
	public:
		static ModelAndView Invalid();
		static ModelAndView CreateFromViewId(ViewId viewId);

	public:
		ModelAndView(ModelAndView&& other) noexcept;

		bool	IsValid() const;
		bool	HasModel() const;
		ViewId	GetViewId() const;
		std::unique_ptr<Model> ReleaseModel();

		void SetModel(std::unique_ptr<Model>&& dataModel);

	private:
		explicit ModelAndView(ViewId viewId);

	private:
		const ViewId m_viewId;
		std::unique_ptr<Model> m_model;
	};
}