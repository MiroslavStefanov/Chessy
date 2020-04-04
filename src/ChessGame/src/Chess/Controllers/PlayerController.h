#pragma once
#include "mvc/BaseController.h"
#include "Chessy.h"

namespace chess
{
	enum class ETurnState : int;

	class PlayerController : public mvc::BaseController, private Chessy
	{
	protected:
		void RegisterConsumers() override;
		mvc::ModelAndView OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event) override;

	private:
		mvc::ModelAndView OnCellClickedEvent(class CellClickedEvent const& event);

	private:
		mvc::ModelAndView MakeChessboardView(ETurnState turnState) const;
	};
}