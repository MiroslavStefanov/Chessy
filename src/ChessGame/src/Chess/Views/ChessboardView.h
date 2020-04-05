#pragma once
#include "Models/ChessboardViewModel.h"
#include "Models/PlayerTurnViewModel.h"
#include "mvc/View.h"

namespace chess
{
	class ChessboardView : public mvc::View
	{
	public:
		void Update(mvc::ModelAndView&& modelAndView) override;
		void ProcessInput(mvc::BaseInputDevice* inputDevice) override;
		void ProcessOutput(mvc::BaseOutputDevice* outputDevice) override;

	private:
		void UpdateBoardModel(mvc::ModelAndView&& modelAndView);
		void UpdateTurnModel(mvc::ModelAndView&& modelAndView);

	private:
		ChessboardViewModel m_boardModel;
		PlayerTurnViewModel m_turnModel;
	};
}