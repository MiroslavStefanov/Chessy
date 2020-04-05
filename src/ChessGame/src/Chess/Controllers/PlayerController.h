#pragma once
#include "mvc/BaseController.h"

namespace chess
{
	enum class ETurnState : int;

	class PlayerController : public mvc::BaseController
	{
	public:
		PlayerController(class PlayerService& playerService);

	protected:
		void RegisterConsumers() override;
		mvc::ModelAndView OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event) override;

	private:
		//mvc::ModelAndView OnCellClickedEvent(class CellClickedEvent const& event);

	private:
		std::unique_ptr<mvc::Model> CreatePlayerTurnViewModel(ETurnState turnState) const;

	private:
		PlayerService& m_playerService;
	};
}