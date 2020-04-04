#pragma once
#include "Models/ChessboardViewModel.h"
#include "mvc/View.h"

namespace chess
{
	class ChessboardView : public mvc::View
	{
	public:
		void Update(const mvc::ModelAndView& modelAndView) override;
		void Render(mvc::BaseOutputDevice& outputDevice) override;

	private:
		std::string TurnStateToString(ETurnState turnState) const;

	private:
		ChessboardViewModel m_dataModel;
	};
}