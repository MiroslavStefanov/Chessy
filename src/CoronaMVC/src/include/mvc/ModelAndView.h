#pragma once

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

		template<class ModelClass>
		const ModelClass* GetModel() const;

		void SetModel(std::unique_ptr<Model>&& dataModel);

	private:
		explicit ModelAndView(ViewId viewId);

	private:
		const ViewId m_viewId;
		std::unique_ptr<Model> m_dataModel;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<class ModelClass>
	inline const ModelClass* ModelAndView::GetModel() const
	{
		return dynamic_cast<const ModelClass*>(m_dataModel.get());
	}
}