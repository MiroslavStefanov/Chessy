#pragma once
#include "Models/ChessboardViewModel.h"
#include "mvc/View.h"

namespace chess
{
	class ChessboardView : public mvc::View
	{
	public:
		void Update(const mvc::ModelAndView& modelAndView) override;
		void ProcessInput(mvc::BaseInputDevice * inputDevice) override;
		void ProcessOutput(mvc::BaseOutputDevice * outputDevice) override;

	private:
		ChessboardViewModel m_dataModel;
	};
}