#include "pch.h"
#include "ModelAndView.h"
#include "Model.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ModelAndView ModelAndView::Invalid()
	{
		return ModelAndView(ViewId::Invalid());
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ModelAndView ModelAndView::CreateFromViewId(ViewId viewId)
	{
		assert(viewId.IsValid());
		return ModelAndView(viewId);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ModelAndView::ModelAndView(ViewId viewId) : m_viewId(viewId), m_modelId(STRING_ID(""))
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ModelAndView::ModelAndView(ModelAndView&& other) noexcept
		: m_viewId(other.m_viewId)
		, m_modelId(other.m_modelId)
		, m_model(std::move(other.m_model))
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ModelAndView::IsValid() const
	{
		return m_viewId.IsValid();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	bool ModelAndView::HasModel() const
	{
		return m_model != nullptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ViewId ModelAndView::GetViewId() const
	{
		return m_viewId;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	StringId ModelAndView::GetModelId() const
	{
		return m_modelId;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<Model> ModelAndView::ReleaseModel()
	{
		return std::move(m_model);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ModelAndView::SetModel(StringId modelId, std::unique_ptr<Model>&& dataModel)
	{
		m_modelId = modelId;
		m_model = std::move(dataModel);
	}
}