#pragma once
#include "StringHash.h"

namespace mvc
{
	class ModelAndView
	{
	public:
		static ModelAndView Invalid();
		static ModelAndView CreateFromViewId(ViewId viewId);

	public:
		ModelAndView(ModelAndView&& other) noexcept;
		~ModelAndView();

		bool	IsValid() const;
		ViewId	GetViewId() const;
		StringId GetModelId() const;

		template<class ModelClass>
		ModelClass* GetModel();

		void SetModel(StringId modelId, std::unique_ptr<Model>&& dataModel);

	private:
		explicit ModelAndView(ViewId viewId);

	private:
		const ViewId m_viewId;
		StringId m_modelId;
		std::unique_ptr<Model> m_model;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class ModelClass>
	inline ModelClass* ModelAndView::GetModel()
	{
		return dynamic_cast<ModelClass*>(m_model.get());
	}
}